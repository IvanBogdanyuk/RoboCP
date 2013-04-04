#pragma once
#include <stdarg.h>
#include "KinectController.h"
#include "KinectSender.h"
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
	  cout << "can't find config.xml! Default config used." << endl;
  }


  KinectBuffer b;
  KinectController c(&b);
  KinectSender s (&config, &b);

  boost::thread_group tgroup;

  tgroup.create_thread ( boost::bind (&KinectController::FakeStart, &c) ); //don't have kinect so made FakeStart func for testing

  tgroup.create_thread ( boost::bind (&KinectSender::Start, &s) );


  tgroup.join_all ();

  return 0;
}