#pragma once

#include <iostream>

#include <QtSerialPort\QSerialPort>
#include <QtSerialPort\QSerialPortInfo>

class ComConnection{

private:
	QSerialPort serial;

public:
	
	ComConnection(){
		//initializing connection
		serial.setPortName("COM3");
		bool errorFlag = true;

		errorFlag &= serial.open(QIODevice::ReadWrite);
		errorFlag &= serial.setBaudRate(QSerialPort::Baud57600);
		errorFlag &= serial.setDataBits(QSerialPort::Data8);
		errorFlag &= serial.setParity(QSerialPort::NoParity);
		errorFlag &= serial.setStopBits(QSerialPort::OneStop);
		errorFlag &= serial.setFlowControl(QSerialPort::NoFlowControl);

		if(!errorFlag) std::cout <<"error during com connection initialization \n";
	}

	void Write(char* message, int length){	//sends the message via com-port

		message[length] = '\0';	//an c-string should be ended by a special symbol

		//preparing and sending the message
		QString string_message((const char*) message);
		QByteArray message_bytes = string_message.toLocal8Bit();
		serial.write(message_bytes);

		//lock thread until bytes are sended
		if(!serial.waitForBytesWritten(50)) 
			std::cout<<"error during sending a packet via com-port: writing time exceeded \n";

		
		if (serial.waitForReadyRead(50)) {
			QByteArray responseData = serial.readAll();
			while (serial.waitForReadyRead(10))
			  responseData += serial.readAll();

			QString response(responseData);
			std::cout<<response.toUtf8().data();
		}
	}

};