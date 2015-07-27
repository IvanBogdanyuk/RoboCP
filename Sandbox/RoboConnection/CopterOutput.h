#pragma once
#include "joystickData.h"
class DataInputController
{
public:
	virtual void WriteMavlinkPacket(MavlinkPacket* packet) = 0;
};

class CopterOutputBuffer
{
public:
	virtual void WriteCopterAttitude(CopterAttitude* attitude) = 0;
	virtual void ReadCopterAttitude(CopterAttitude* attitude) = 0;
};

class CopterOutputController
{
public:
	virtual void GetLastCopterAttitude(CopterAttitude* attitude) = 0;
};

class OneElementCopterOutputBuffer : public CopterOutputBuffer
{
public:
	OneElementCopterOutputBuffer();
	void WriteCopterAttitude(CopterAttitude* attitude);
	void ReadCopterAttitude(CopterAttitude* attitude);

private:
	DataHandler<CopterAttitude>* m_copterAttitudeHandler;
};

class CopterOutputControllerImpl : public CopterOutputController
{
public:
	CopterOutputControllerImpl(CopterOutputBuffer* buffer);
	void GetLastCopterAttitude(CopterAttitude* attitude);

private:
	CopterOutputBuffer* m_buffer;
};

class LinkerToBufferController : public DataInputController
{
public:
	LinkerToBufferController(CopterOutputBuffer* buffer);
	void WriteMavlinkPacket(MavlinkPacket* packet);
	
private:
	void writeAttitudePacket(MavlinkPacket* packet);

	CopterOutputBuffer* m_buffer;
};