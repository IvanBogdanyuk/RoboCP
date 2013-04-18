#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <queue>

#include "KinectData.h"
#include "NanoReceived.h"
#include "ArduCopterReceived.h"
#include "CameraReceived.h"
#include "Command.h"
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
  boost::interprocess::interprocess_mutex* mtx; // Mutex for thread safety

public:
  boost::interprocess::interprocess_semaphore* Vacant; // Count of vacant places
  boost::interprocess::interprocess_semaphore* Used; // Count of used places
   
  ReceivedBuffer(int size);

  ~ReceivedBuffer(void);

  void Enqueue(Type a); // Add to buffer

  Type Dequeue(); // Receive from buffer
};
