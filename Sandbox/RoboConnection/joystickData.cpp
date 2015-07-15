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

void HeartBeat::toMavlinkPacket(MavlinkPacket* result, MavlinkVisitor* visitor)
{
	visitor->visitHeartBeat(result);
}

void JoystickData::toMavlinkPacket(MavlinkPacket* result, MavlinkVisitor* visitor)
{
	visitor->visitRc_Channels_Override(result, pitch, roll, gas, rudder);
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

void JoystickData::print()
{
	std::cout << "JoystickData: \nrudder:" << rudder << "\ngas: " << gas << "\npitch: " << pitch << "\nroll: " << roll << "\n\n";
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

	jbuffers[!activeBuffer].toMavlinkPacket(packet, visitor);	//bug

	isReading -= 1;
	readingCondition.wakeAll();
	mutex.unlock();
}

void SingleJoystickBuffer::read(MavlinkPacket* packet, MavlinkVisitor* visitor)
{
	if (heartBitTimer.elapsed() > 1000)
	{
		heartbeat->toMavlinkPacket(packet, visitor);
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
	midJoystickData.toMavlinkPacket(packet, visitor);
}
void CircularJoystickBuffer::read(MavlinkPacket* packet, MavlinkVisitor* visitor){
	if (heartBitTimer.elapsed() > 1000)
	{
		heartbeat->toMavlinkPacket(packet, visitor);
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

