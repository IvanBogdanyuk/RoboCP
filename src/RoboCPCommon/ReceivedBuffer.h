#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <queue>

#include "KinectData.h"
#include "NanoReceived.h"
#include "ArduCopterReceived.h"
#include "Send.h"


#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/asio.hpp>

using namespace std;

template <class Type>
class ReceivedBuffer
{
private:
	queue<Type> queue;
	boost::interprocess::interprocess_mutex* mtx;
public:
  boost::interprocess::interprocess_semaphore* Vacant;
  boost::interprocess::interprocess_semaphore* Used;
   
  ReceivedBuffer(int size);

  ~ReceivedBuffer(void);

  void Enqueue(Type a);

  Type Dequeue();
};
