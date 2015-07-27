#include "joystickData.h"
#include "math.h"

int epsilon = 200;

int weak_sign(double arg, double epsilond)
{
	if (arg > 0) if (arg > epsilond) return 1; else return 0;
	else if (arg < -epsilond) return -1; else return 0;
}

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

bool JoystickData::isGasZero()
{
	if (gas < 100 + epsilon) return true;
	return false;
}

bool JoystickData::isNonZero()
{
	bool isNonZero = false;
	isNonZero |= ((pitch > 1500 + epsilon) || ((pitch < 1500 - epsilon)));
	isNonZero |= ((roll > 1500 + epsilon) || ((roll < 1500 - epsilon)));
	isNonZero |= ((rudder > 1500 + epsilon) || ((rudder < 1500 - epsilon)));

	return isNonZero;
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

JoystickDataPool::JoystickDataPool(int size)
{
	m_size = size;
	m_freeData = new bool[size];
	m_data = new JoystickData[size];

	for (int i = 0; i < size; i++)
	{
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
			m_freeData[toHandle] = false;

			return &m_data[toHandle];   //return that cell
		}
		
		count++;
	} while (count < m_size);

	return 0;
}
void JoystickDataPool::FreeJoystickData(JoystickData* data)
{
	int index = (data - m_data) / sizeof(bool);    //computes index of a cell based on adresses difference
	m_freeData[index] = true;
}

JoystickDataPool::~JoystickDataPool()
{
	delete[] m_freeData;
	
	delete[] m_data;
}

QueuedControlBuffer::QueuedControlBuffer(int max_size){
	m_size = max_size;
	m_dataPool = new JoystickDataPool(m_size);
	m_joystickDataQueue = new TSDataHandler<JoystickData*>(m_size);

	m_joystickDataQueue->Write(new JoystickData());
}

JoystickData* QueuedControlBuffer::Read(){
	bool notEmpty = m_joystickDataQueue->Peek(m_workData);
	if (!notEmpty) return 0;
	return m_workData;
}

void QueuedControlBuffer::Write(JoystickData* jdata){
	if (m_joystickDataQueue->Size() < m_size)
	{
		JoystickData* toWrite = m_dataPool->GetJoystickData();
		if (toWrite == 0)
		{
			std::cout << "something went wrong in buffer::write()\n";
			return;
		}
		jdata->copy(toWrite);
		m_joystickDataQueue->Write(toWrite);
	}
	else std::cout << "buffer is full" << m_joystickDataQueue->Size() << std::endl;
}
int QueuedControlBuffer::Size(){
	return m_joystickDataQueue->Size();
}

void QueuedControlBuffer::Next()
{
	m_joystickDataQueue->Read(m_workData);
	m_dataPool->FreeJoystickData(m_workData);
}

int QueuedControlBuffer::GetMaxSize()
{
	return m_size;
}

JoystickToBufferController::JoystickToBufferController(ControlBuffer* buffer)
{
	SetControlBuffer(buffer);
}

void JoystickToBufferController::WriteJoystickData(JoystickData* jdata){
	m_buffer->Write(jdata);
}

void JoystickToBufferController::SetControlBuffer(ControlBuffer* buffer){
	m_buffer = buffer;
}

int JoystickToBufferController::MsToWait()
{
	return (int) 75.0*m_buffer->Size() / m_buffer->GetMaxSize();
}

BufferToLinkerController::BufferToLinkerController(ControlBuffer* buffer, int rate) : m_heartbeat()
{
	SetControlBuffer(buffer);
	m_rate = rate;
	m_meanJoystickData = new JoystickData();

	m_heartBitTimer.start();
}
void BufferToLinkerController::Read(MavlinkPacket* packet, MavlinkVisitor* visitor)
{
	if (m_heartBitTimer.elapsed() > 1000)
	{
		m_heartbeat.ToMavlinkPacket(packet, visitor);
		m_heartBitTimer.restart();
	}
	else
	{
		prepareJData();
		visitor->VisitRc_Channels_Override(packet, m_meanJoystickData->pitch, m_meanJoystickData->roll,
			m_meanJoystickData->gas, m_meanJoystickData->rudder);
	}
}
void BufferToLinkerController::SetControlBuffer(ControlBuffer* buffer)
{
	m_buffer = buffer;
}


JoystickData* BufferToLinkerController::prepareJData()
{
	m_meanJoystickData->setZero();
	int count = 0;
		

	while (count < m_rate)
	{
		m_workData = m_buffer->Read();    //reading from the buffer to some buffer var
		//finding mean in the queue in cycle
		m_meanJoystickData->gas = (count*m_meanJoystickData->gas + m_workData->gas) / (count + 1);
		m_meanJoystickData->pitch = (count*m_meanJoystickData->pitch + m_workData->pitch) / (count + 1);
		m_meanJoystickData->roll = (count*m_meanJoystickData->roll + m_workData->roll) / (count + 1);
		m_meanJoystickData->rudder = (count*m_meanJoystickData->rudder + m_workData->rudder) / (count + 1);

		count++;

		if (m_buffer->Size() > 1) m_buffer->Next();	//last object remains in buffer
		else break;
	}
	if (m_buffer->Size() == 0){
		std::cout << "emptiing buffer\n";
	}
	return m_meanJoystickData;
}

void Joystick::checkData(JoystickData* data)
{
	if (data->gas > 1025)
		m_began = true;
	if ((data->gas < 1025) && (m_began))
		m_danger = true;
}

bool Joystick::isDanger()
{
	return m_danger;
}

bool Joystick::hasBegun()
{
	return m_began;
}

CrossPoint2D::CrossPoint2D()
{
	m_x = 0.0;
	m_y = 0.0;
}

CrossPoint2D::CrossPoint2D(CrossPoint2D& point)
{
	m_x = point.GetX();
	m_y = point.GetY();
}

CrossPoint2D& CrossPoint2D::operator=(CrossPoint2D &arg)
{
	m_x = arg.GetX();
	m_y = arg.GetY();

	return *this;
}

double CrossPoint2D::GetX()
{
	return m_x;
}

double CrossPoint2D::GetY()
{
	return m_y;
}

void CrossPoint2D::SetXY(double x, double y)
{
	m_x = x;
	m_y = y;
}

SimpleProportionalCrossStabilizer::SimpleProportionalCrossStabilizer(double factor)
{
	m_factor = factor;
	m_pointContainer = new OneElementDataHandler<CrossPoint2D>();
}

bool SimpleProportionalCrossStabilizer::isDanger()
{
	return false; //won't be used
}

bool SimpleProportionalCrossStabilizer::hasBegun()
{
	return true;	//won't be used
}

void SimpleProportionalCrossStabilizer::GetJoysticState(JoystickData* data)
{
	m_pointContainer->Read(m_workPoint);

	data->pitch = 1500 + m_workPoint.GetY()*m_factor;
	data->roll = 1500 + m_workPoint.GetX()*m_factor;
	data->rudder = 1500;
}

DataHandler<CrossPoint2D>* SimpleProportionalCrossStabilizer::GetPointContainer()
{
	return m_pointContainer;
}

RelayCrossStabilizer::RelayCrossStabilizer(double factor, double stableRadius)
{
	m_factor = factor;
	m_stableRadius = stableRadius;
	m_pointContainer = new OneElementDataHandler<CrossPoint2D>();

	if (m_stableRadius < 0) m_stableRadius = -m_stableRadius;
}

bool RelayCrossStabilizer::isDanger()
{
	return false; //won't be used
}

bool RelayCrossStabilizer::hasBegun()
{
	return true;	//won't be used
}

void RelayCrossStabilizer::GetJoysticState(JoystickData* data)
{
	m_pointContainer->Read(m_workPoint);

	data->pitch = 1500 + (uint16_t) weak_sign(m_workPoint.GetY(), m_stableRadius)*m_factor;
	data->roll = 1500 + (uint16_t) weak_sign(m_workPoint.GetX(), m_stableRadius)*m_factor;
	data->rudder = 1500;
}

DataHandler<CrossPoint2D>* RelayCrossStabilizer::GetPointContainer()
{
	return m_pointContainer;
}

ControlSwitcher::ControlSwitcher(CrossStabilizer* stabilizer, Joystick* externJoystick)
{
	m_stabilizer = stabilizer;
	m_externJoystick = externJoystick;

	m_danger = false;
	m_began = false;
}

DataHandler<CrossPoint2D>* ControlSwitcher::GetPointContainer()
{
	return m_stabilizer->GetPointContainer();
}

void ControlSwitcher::GetJoysticState(JoystickData* data)
{
	m_externJoystick->GetJoysticState(data);	//check the real joystick first
	checkData(data);	//needed for red button working

	if (!data->isNonZero())	//if the real joystick isn't in use
	{
		m_stabilizer->GetJoysticState(data);	//check software control input
	}
}