#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <queue>

#include "KinectData.h"
#include "NanoReceived.h"
#include "ArduCopterReceived.h"

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/asio.hpp>

using namespace std;

template <class Type>
class ReceivedBuffer
{
public:
  queue<Type> q;
  boost::interprocess::interprocess_semaphore* Vacant;
  boost::interprocess::interprocess_semaphore* Used;
  boost::interprocess::interprocess_mutex* Mtx;
  
  ReceivedBuffer(int size);

  ~ReceivedBuffer(void);

  void Enqueue(Type a);

  Type Dequeue();
};
