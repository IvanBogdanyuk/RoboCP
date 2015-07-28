#include "CopterOutput.h"

OneElementCopterOutputBuffer::OneElementCopterOutputBuffer()
{
	m_copterAttitudeHandler = new OneElementDataHandler<CopterAttitude>();
}

void OneElementCopterOutputBuffer::WriteCopterAttitude(CopterAttitude* attitude)
{
	m_copterAttitudeHandler->Write(*attitude);
}

void OneElementCopterOutputBuffer::ReadCopterAttitude(CopterAttitude* attitude)
{
	m_copterAttitudeHandler->Read(*attitude);
}

CopterOutputControllerImpl::CopterOutputControllerImpl(CopterOutputBuffer* buffer)
{
	m_buffer = buffer;
}

void CopterOutputControllerImpl::GetLastCopterAttitude(CopterAttitude* attitude)
{
	m_buffer->ReadCopterAttitude(attitude);
}

LinkerToBufferController::LinkerToBufferController(CopterOutputBuffer* buffer)
{
	m_buffer = buffer;
}
void LinkerToBufferController::WriteMavlinkPacket(MavlinkPacket* packet)
{
	unsigned char type = packet->data[5];
	
	if (type == 22) //mavlink param value
		std::cout << "param val";
	if (type == 253) //status text
		std::cout << "status val";
	int idd = type;
	std::cout << "read packet with id: " << idd << std::endl;

	switch (type)
	{
	case 30: {writeAttitudePacket(packet); break;}
	}
}

void LinkerToBufferController::writeAttitudePacket(MavlinkPacket* packet)
{
	//TODO: implement
}