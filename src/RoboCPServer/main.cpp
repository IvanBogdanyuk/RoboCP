#pragma once
#include <stdarg.h>
#include "KinectViewer.h"
#include "SendReceiver.h"
#include "CommandMaker.h"

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

  KinectViewer v (&config);
  CommandMaker m (&config);

  SendBuffer b (50);
  SendReceiver s (&config, &b);

  boost::thread_group tgroup;

  tgroup.create_thread ( boost::bind (&KinectViewer::Start, &v) );

  tgroup.create_thread ( boost::bind (&CommandMaker::Start, &m) );

  tgroup.create_thread ( boost::bind (&SendReceiver::Start, &s) );


  tgroup.join_all ();

  return 0;
}