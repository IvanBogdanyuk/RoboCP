#include <QtSerialPort/QSerialPort>
#include "qelapsedtimer.h"
#include "robo_mavlink_test\mavlink.h"
#include <iostream>
#include "time.h"
using namespace std;
typedef unsigned char uint8_t;

QSerialPort serial;
QList<int> sequence;
mavlink_system_t mavlink_system;
mavlink_system_t mavlink_target;
uint8_t system_type = 6; //MAV_TYPE
uint8_t autopilot_type = MAV_AUTOPILOT_INVALID; //MAV_AUTOPILOT ok

uint8_t system_mode = MAV_MODE_PREFLIGHT; //MAV_MODE_PREFLIGHT ok
uint32_t custom_mode = 0;                 //Äëÿ MAV_MODE_FLAG ok     
uint8_t system_state = MAV_STATE_STANDBY; //MAV_STATE ok
QByteArray readPacket()
{

	QByteArray buffer;
	int count = 0;
	int length = 0;
	int readcount = 0;
	string buildplaintxtline, plaintxtline;
	while (serial.isOpen())
	{
		if (readcount > 300)
		{
			std::cout << "MAVLink readpacket No valid mavlink packets";
			break;
		}
		readcount++;
		if (serial.waitForReadyRead(-1))
		{
			/*for (int i = 0; i < t.size(); ++i)
			{
				cout << (unsigned char)t[i];
				if ((unsigned char)t[i] == 254)
					cout << endl << "BAB" << endl;
			}
			*/
			buffer.replace(count, 1, serial.read(1));

		}
		// check if looks like a mavlink packet and check for exclusions and write to console
		if ((unsigned char)buffer[0] != 254 && buffer[0] != 'U')
		{
			if ((unsigned char)buffer[0] >= 0x20 && (unsigned char)buffer[0] <= 127 || buffer[0] == '\n' || buffer[0] == '\r')
			{
				// check for line termination
				if (buffer[0] == '\r' || buffer[0] == '\n')
				{
					// check new line is valid
					if (buildplaintxtline.length() > 3)
						plaintxtline = buildplaintxtline;

					// reset for next line
					buildplaintxtline = "";
				}
				buildplaintxtline += (char)buffer[0];
			}
			count = 0;
			buffer[1] = 0;
			continue;
		}
		// reset count on valid packet
		readcount = 0;
		
		//Console.WriteLine(DateTime.Now.Millisecond + " SR2 " + BaseStream.BytesToRead);

		// check for a header
		if ((unsigned char)buffer[0] == 254 || buffer[0] == 'U')
		{
			// if we have the header, and no other chars, get the length and packet identifiers
			if (count == 0)
			{
				if (serial.waitForReadyRead(-1))
				{
					buffer.append(serial.read(5));
					count = 5;//?
				}
			}

			// packet length
			length = buffer[1] + 6 + 2 - 2; // data + header + checksum - U - length
			if (count >= 5)
			{
				if (serial.waitForReadyRead(-1))
				{
					buffer.append(serial.read(length - 4));
				}
				count = length + 2;
			}
			break;
		}
		count++;
		if (count == 299)
			break;
	}
	sequence.append((unsigned char)buffer[2]);
	// packet is now verified

	char sysid = buffer[3];
	char compid = buffer[4];

	if ((buffer[0] == 'U' || (unsigned char)buffer[0] == 254) && buffer.length() >= buffer[1])
	{
		// check if we lost pacakets based on seqno

		// set seens sysid's based on hb packet - this will hide 3dr radio packets
		if ((unsigned char)buffer[5] == 0)
		{
			//its hb
		}

		if ((unsigned char)buffer[5] == 253) // status text
		{
			//its status text
		}
		
	}
	
	return buffer;
}
long t = time(NULL);

QByteArray getHeartBit()
{

	int readcount = 0;
	while (true)
	{
		QByteArray buffer = readPacket();
		readcount++;
		if ((unsigned char)buffer[5] == 0)
		{
			return buffer;
		}
	}
}
void sendHeartBit()
{
	mavlink_message_t msg;               // MAVLink ñîîáùåíèå
	uint8_t buf[MAVLINK_MAX_PACKET_LEN];
	mavlink_msg_heartbeat_pack(mavlink_system.sysid, mavlink_system.compid, &msg, system_type, autopilot_type, system_mode, custom_mode, system_state);

	uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
	serial.write((char*)buf, len);
}
void getParamList()
{
	

	mavlink_system.sysid = 255;    //ID îòïðàâëÿþùåé ñèñòåìû
	mavlink_system.compid = 190;	//Êîìïîíåíò îòïðàâëÿþùåé ñèñòåìû

	mavlink_target.sysid = 1;      //ID ïðèíèìàþùåé ñèñòåìû
	mavlink_target.compid = 1;		//Êîìïîíåíò ïðèíèìàþùåé ñèñòåìû

	
	// Âåðñèÿ ïðîòîêîëà MAVLink: 3
	cout << "sleeping" << endl << endl;
	//_sleep(1000);
	cout << "wake up" << endl << endl;
	mavlink_message_t msg;               // MAVLink ñîîáùåíèå
	uint8_t buf[MAVLINK_MAX_PACKET_LEN];
	mavlink_msg_param_request_list_pack(mavlink_system.sysid, mavlink_system.compid,
		&msg, mavlink_target.sysid, mavlink_target.compid);
	uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
	serial.write((char*)buf, len);
	
	int packets = 0;
	int param_count = 10;
	do
	{
		QByteArray buffer = readPacket();
		packets++;
		if ((unsigned char)buffer[5] == 22) //mavlink param value
		{
			cout << "param val";
		}
		if ((unsigned char)buffer[5] == 253) //status text
		{
			cout << "status val";
		}
		int idd = (unsigned char)buffer[5];
		cout << "got obj with id: " << idd << endl;
	} while (1);
}

int main()
{



	QString serialPortName = "COM5";
	serial.setPortName(serialPortName);
	int serialPortBaudRate = QSerialPort::Baud57600;

	serial.setBaudRate(serialPortBaudRate);
	//_sleep(1000);
	if (!serial.open(QIODevice::ReadWrite))
	{
		cout << QObject::tr("Failed to open port %1, error: %2").arg(serialPortName).arg(serial.error()).toStdString() << endl;
	}
	

	//serial.flush();
	getHeartBit();
	int lastsize = 0;
	QByteArray readData;
	QElapsedTimer timer;
	int lasttime = 0;
	timer.start();
	getParamList();
	while (1)
	{

	}
	/*
	while (1)
	{
		QByteArray hb= getHeartBit();
		int curtime = timer.elapsed();
		cout <<"got heartbit"<<" at: " <<curtime<<" ms  "<<" diff: "<<curtime-lasttime<<" ms"<<endl;
		lasttime = curtime;
	}
	*/
		/*
	while (true)
	{
	int iii=serial.read(readData.data(),2);
	lastsize++;
	if (iii != 0)
	cout << "ASDNASKND";
	if ((unsigned char)readData[lastsize] == 254)
	cout << "123";
	}









	while (serial.waitForReadyRead(5000))
	{

	for (int i = lastsize; i < readData.size(); ++i)
	{
	if ((unsigned char)readData[i] == 254)
	{
	cout << "---" << endl;
	int length = (unsigned char)readData[i + 1] + 6 + 2 - 2;

	int id = (unsigned char)readData[i + 5];
	cout << "ID :  "<<id << "   ";
	cout << "---" << endl;
	i += length;
	}
	}
	lastsize += readData.size();
	QByteArray t = serial.read(1);
	for (int i = 0; i < t.size(); ++i)
	cout << (unsigned char)t[i];
	readData.append(t);
	}
	*/
	int i;
	cin >> i;
	return 0;
}
