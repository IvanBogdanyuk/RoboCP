#pragma once
#include <stdarg.h>
#include "KinectViewer.h"
#include "KinectReceiver.h"
#include "SendReceiver.h"
#include "CommandMaker.h"
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include <glog/raw_logging.h>

int main(char *args[], int count)
{
  freopen ("log.log", "a", stderr);
  RAW_LOG (INFO, "MAIN SERVER STARTED");

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
  
  KinectReceiver r (&config, &v);

  CommandMaker m (&config);

  SendReceiver s (&config, &v);


  boost::thread_group tgroup;

  tgroup.create_thread ( boost::bind (&KinectViewer::Start, &v) );

  tgroup.create_thread ( boost::bind (&KinectReceiver::Start, &r) );

  tgroup.create_thread ( boost::bind (&CommandMaker::Start, &m) );

  tgroup.create_thread ( boost::bind (&SendReceiver::Start, &s) );


  tgroup.join_all ();

  return 0;
}