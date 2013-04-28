#pragma once
#include <stdarg.h>
#include "KinectViewer.h"
#include "KinectReceiver.h"
#include "KinectManager.h"
#include "SendReceiver.h"
#include "SendManager.h"
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

  KinectBuffer kinectBuffer (10);
  KinectViewer kinectViewer (&config);
  KinectReceiver kinectReceiver (&config, &kinectBuffer);
  KinectManager kinectManager (&kinectBuffer, &kinectViewer);

  CommandMaker commandMaker (&config);

  SendBuffer sendBuffer (50);
  SendReceiver sendReceiver (&config, &sendBuffer);
  SendManager sendManager (&sendBuffer, &kinectViewer);



  boost::thread_group tgroup;

  tgroup.create_thread ( boost::bind (&KinectViewer::Start, &kinectViewer) );

  tgroup.create_thread ( boost::bind (&KinectReceiver::Start, &kinectReceiver) );

  tgroup.create_thread ( boost::bind (&KinectManager::Start, &kinectManager) );

  tgroup.create_thread ( boost::bind (&CommandMaker::Start, &commandMaker) );

  tgroup.create_thread ( boost::bind (&SendReceiver::Start, &sendReceiver) );

  tgroup.create_thread ( boost::bind (&SendManager::Start, &sendManager) );


  tgroup.join_all ();

  return 0;
}