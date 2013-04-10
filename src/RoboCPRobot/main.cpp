#pragma once
#include <stdarg.h>
#include "KinectController.h"
#include "KinectDownsampler.h"
#include "KinectSender.h"
#include "NanoController.h"
#include "ClientReceiver.h"
#include "XMLConfig.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>

int main(char *args[], int count)
{

  XMLConfig config;
  { //deserialization
    std::ifstream ifs("config.xml");
	if (ifs.is_open()) {
	  boost::archive::xml_iarchive ia(ifs);
	  ia >> BOOST_SERIALIZATION_NVP(config);
	}
	else
	  cout << "Can't find config.xml! Default config used." << endl;
  }

  KinectBuffer b1(10);
  KinectBuffer b2(10);
  KinectController c(&b1);
  KinectDownsampler d (&b1, &b2);
  KinectSender s (&config, &b2);
  ClientReceiver r (&config);

  NanoReceivedBuffer NanoBuffer(1000);
  NanoController  NanoControl(&NanoBuffer);
  
  boost::thread_group tgroup;

  tgroup.create_thread ( boost::bind (&KinectController::FakeStart, &c) ); //don't have kinect so made FakeStart func for testing

  tgroup.create_thread ( boost::bind (&KinectDownsampler::Start, &d) );

  tgroup.create_thread ( boost::bind (&KinectSender::Start, &s) );

  tgroup.create_thread ( boost::bind (&ClientReceiver::Start, &r) );
  
  tgroup.create_thread ( boost::bind (&NanoController::Start, &NanoControl) );

  tgroup.join_all ();

  return 0;
}