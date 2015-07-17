#include "joystickData.h"

MavlinkPacket::MavlinkPacket()
{
	data = new unsigned char[MAVLINK_MAX_PACKET_LEN];    //Maximum length of the mavlink package
}
void MavlinkPacket::toString()                           //
{
	int length = ((unsigned short)data[1]) + 7;

	for (int i = 0; i < length; i++){
		std::cout << (unsigned short)data[i] << "\t";
	}

	std::cout << "\n";
}

void HeartBeat::ToMavlinkPacket(MavlinkPacket* result, MavlinkVisitor* visitor)
{
	visitor->VisitHeartBeat(result);
}

JoystickData* JoystickData::clone()
{
	return new JoystickData(rudder, gas, pitch, roll);
}

void JoystickData::copy(JoystickData* to)
{
	to->rudder = rudder;
	to->gas = gas;
	to->pitch = pitch;
	to->roll = roll;
}

void JoystickData::setZero(){
	this->gas = 0;
	this->pitch = 0;
	this->roll = 0;
	this->rudder = 0;
}

void JoystickData::print()
{
	std::cout << "JoystickData: \nrudder:" << rudder << "\ngas: " << gas << "\npitch: " << pitch << "\nroll: " << roll << "\n\n";
}

void JoystickData::ToMavlinkPacket(MavlinkPacket* result, MavlinkVisitor* visitor)
{
	visitor->VisitRc_Channels_Override(result, pitch, roll, gas, rudder);
}

SingleJoystickBuffer::SingleJoystickBuffer()
{
	jbuffers = new JoystickData[2];

	activeBuffer = 0;

	isReading = false;

	heartbeat = new HeartBeat();
	lastHeartbeat = -1;
	heartBitTimer.start();
	joystickTimer.start();
}

void SingleJoystickBuffer::switchBuffer()
{
	if (activeBuffer) activeBuffer = 0;
	else activeBuffer = 1;
}

int SingleJoystickBuffer::writeJoystickData(JoystickData* jdata)
{
	jdata->copy(&jbuffers[activeBuffer]);

	mutex.lock();

	while (isReading) readingCondition.wait(&mutex);
	switchBuffer();

	mutex.unlock();

	return 0;
}

void SingleJoystickBuffer::readJoystickData(MavlinkPacket* packet, MavlinkVisitor* visitor)
{
	mutex.lock();
	isReading++;

	jbuffers[!activeBuffer].ToMavlinkPacket(packet, visitor);	//bug

	isReading -= 1;
	readingCondition.wakeAll();
	mutex.unlock();
}

void SingleJoystickBuffer::read(MavlinkPacket* packet, MavlinkVisitor* visitor)
{
	if (heartBitTimer.elapsed() > 1000)
	{
		heartbeat->ToMavlinkPacket(packet, visitor);
		heartBitTimer.restart();
	}
	else
	if (joystickTimer.elapsed() > 59)
	{
		readJoystickData(packet, visitor);
		joystickTimer.restart();
	}
}

SingleJoystickBuffer::~SingleJoystickBuffer()
{
	delete[] jbuffers;
}

CircularJoystickBuffer::CircularJoystickBuffer(int array_size)
{
	size = array_size;
	i = 0;
	j = 0;

	firstBuffer = new JoystickData[size];
	secondBuffer = new JoystickData[size];
	firstSent = new bool[size];
	secondSent = new bool[size];
	for (int k = 0; k < size; k++){
		firstSent[k] = 1;
		secondSent[k] = 1;
	}

	isReading = false;
	writeToSecondBuffer = false;
	heartbeat = new HeartBeat();
	lastHeartbeat = -1;

	heartBitTimer.start();
	joystickTimer.start();

	secondBuffLoad = 0;
	factorToWait = 1.3;
}
void CircularJoystickBuffer::flushToFirstBuffer()
{
	for (int k = j; k < (j + size); ++k)
	{
		if (!secondSent[k % size])
		{
			secondBuffer[k % size].copy(&firstBuffer[i]);
			firstSent[i] = 0;
			++i;
			i %= size;
			secondSent[k % size] = 1;

		}
	}

	secondBuffLoad = 0;
}

int CircularJoystickBuffer::writeJoystickData(JoystickData* jData)
{
	if (isReading)
	{
		secondMutex.lock();
		writeToSecondBuffer = true;
		if (secondSent[j])  //second buffer is not full
		{
			jData->copy(&secondBuffer[j]);
			secondSent[j] = 0;
			++j;
			j %= size;
			this->secondBuffLoad++;
		}
		else  //second bufer is full
			std::cout << "Second buffer is full!" << std::endl;
		secondMutex.unlock();
	}
	else
	{
		firstMutex.lock();
		writeToSecondBuffer = true;
		if (firstSent[i]) //firstbuffer is not full
		{
			jData->copy(&firstBuffer[i]);
			firstSent[i] = 0;
			++i;
			i %= size;
		}
		else
		{
			if (secondSent[j])
			{
				jData->copy(&secondBuffer[j]);
				secondSent[j] = 0;
				++j;
				j %= size;
				this->secondBuffLoad++;
			}
			else
				std::cout << " First and second buffer is full!" << std::endl;
		}
		firstMutex.unlock();
	}
	return floor(factorToWait*secondBuffLoad);
}
void CircularJoystickBuffer::getMiddleValue(JoystickData* midJoystickData, int i)
{
	double rudder = 0, pitch = 0, roll = 0, gas = 0;
	int sum = 0;
	i += size;
	for (int k = 0; k < size; ++k)
	{
		if (!firstSent[k])
		{
			rudder += firstBuffer[(i - k - 1) % size].rudder;
			pitch += firstBuffer[(i - k - 1) % size].pitch;
			roll += firstBuffer[(i - k - 1) % size].roll;
			gas += firstBuffer[(i - k - 1) % size].gas;
			++sum;
		}
	}
	if (sum != 0)
	{

		rudder /= sum;
		pitch /= sum;
		roll /= sum;
		gas /= sum;
	}
	else
	{
		rudder = firstBuffer[(i - 1) % size].rudder;
		pitch = firstBuffer[(i - 1) % size].pitch;
		roll = firstBuffer[(i - 1) % size].roll;
		gas = firstBuffer[(i - 1) % size].gas;
	}
	midJoystickData->rudder = (uint16_t)abs(rudder);
	midJoystickData->pitch = (uint16_t)abs(pitch);
	midJoystickData->roll = (uint16_t)abs(roll);
	midJoystickData->gas = (uint16_t)abs(gas);
}
void CircularJoystickBuffer::fill(bool *firstSent)
{
	int k = size + i - 1;
	while (!firstSent[k % size])
	{
		firstSent[k % size] = 1;
		--k;
	}

}
void CircularJoystickBuffer::readJoystickData(JoystickData* midJoystickData)
{
	firstMutex.lock();
	++isReading;


	getMiddleValue(midJoystickData, i);

	fill(firstSent);

	if (!secondSent[(j + size - 1) % size])
	{
		secondMutex.lock();
		flushToFirstBuffer();
		secondMutex.unlock();
	}

	else
		firstSent[(i + size - 1) % size] = 0;
	isReading -= 1;


	firstMutex.unlock();

}
void CircularJoystickBuffer::readJoystickData(MavlinkPacket* packet, MavlinkVisitor* visitor)
{
	readJoystickData(&midJoystickData);
	midJoystickData.ToMavlinkPacket(packet, visitor);
}
void CircularJoystickBuffer::read(MavlinkPacket* packet, MavlinkVisitor* visitor){
	if (heartBitTimer.elapsed() > 1000)
	{
		heartbeat->ToMavlinkPacket(packet, visitor);
		heartBitTimer.restart();
	}
	else
	if (joystickTimer.elapsed() > 59)
	{
		readJoystickData(packet, visitor);
		joystickTimer.restart();
	}
}
CircularJoystickBuffer::~CircularJoystickBuffer()
{
	delete[] firstBuffer;
	delete[] secondBuffer;
	delete[] firstSent;
	delete[] secondSent;
}

JoystickDataPool::JoystickDataPool(int size)
{
	m_size = size;
	m_freeData = new bool[size];
	m_data = new JoystickData*[size];

	for (int i = 0; i < size; i++)
	{
		m_data[i] = new JoystickData();
		m_freeData[i] = true;
	}

	m_lastHandled = 0;
}

JoystickData* JoystickDataPool::GetJoystickData()
{
	int count = 0;	//if equals m_size then there is no free cells
	do
	{
		int toHandle = (m_lastHandled + 1) % m_size;	//index of a cell to handle
		if (m_freeData[toHandle])    //if that cell free
		{
			m_lastHandled = toHandle;
			return m_data[toHandle];   //return that cell
		}
		
		count++;
	} while (count < m_size);

	return 0;
}
void JoystickDataPool::FreeJoystickData(JoystickData* data)
{
	int index = (data - m_data[0]) / sizeof(JoystickData*);    //computes index of a cell based on adresses difference
	m_freeData[index] = true;
}

JoystickDataPool::~JoystickDataPool()
{
	delete[] m_freeData;
	
	for (int i = 0; i < m_size; i++)
	{
		delete m_data[i];
	}

	delete[] m_data;

}
DataSeparateController::DataSeparateController(int size)
{
	m_dataPool = new JoystickDataPool(size);
	m_joystickDataQueue = new TSDataHandler<JoystickData*>(size);
	m_joystickDataQueue->Write(new JoystickData());
	m_meanJoystickData = new JoystickData();
	m_size = size;
}
int DataSeparateController::WriteJoystickData(JoystickData* jdata){
	if (m_joystickDataQueue->Size() < m_size)
	{
		JoystickData* toWrite = m_dataPool->GetJoystickData();
		jdata->copy(toWrite);
		m_joystickDataQueue->Write(toWrite);
	}
	else std::cout << "buffer is full \n";

	return (int) floor(4.0*this->m_joystickDataQueue->Size()/this->m_size);
}
JoystickData* DataSeparateController::prepareJData()
{
	m_meanJoystickData->setZero();
	int count = 0;
	while (count < m_size)
	{
		bool notEmpty = m_joystickDataQueue->Peek(m_workData);	//reading from queue to some buffer var

#ifdef DEBUG
		if (!notEmpty) //if true - bad initialization
		{	 
			std::cout << "reading from empty buffer \n";
			break;
		}
#endif

		//finding mean in the queue in cycle
		m_meanJoystickData->gas = (count*m_meanJoystickData->gas + m_workData->gas) / (count + 1);
		m_meanJoystickData->pitch = (count*m_meanJoystickData->pitch + m_workData->pitch) / (count + 1);
		m_meanJoystickData->roll = (count*m_meanJoystickData->roll + m_workData->roll) / (count + 1);
		m_meanJoystickData->rudder = (count*m_meanJoystickData->rudder + m_workData->rudder) / (count + 1);

		count++;
		m_dataPool->FreeJoystickData(m_workData);    //freing memory

		if (m_joystickDataQueue->Size() > 1) m_joystickDataQueue->Read(m_workData);	//last object remains in queue
		else break;
	}

	return m_meanJoystickData;
}

void DataSeparateController::Read(MavlinkPacket* packet, MavlinkVisitor* visitor)
{
	prepareJData();
	visitor->VisitRc_Channels_Override(packet, m_meanJoystickData->pitch, m_meanJoystickData->roll, m_meanJoystickData->gas, m_meanJoystickData->rudder);
}

DataSeparateController::~DataSeparateController()
{
	delete m_joystickDataQueue;
	delete m_meanJoystickData;
	delete m_dataPool;
}