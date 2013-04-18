#pragma once

//#define FLOW_TEST
//#define GPS_TEST

#include <stdarg.h>
#include "KinectController.h"
#include "KinectDownsampler.h"
#include "KinectSender.h"
#include "NanoController.h"
#include "ArduCopterController.h"
#include "CameraController.h"
#include "ClientReceiver.h"
#include "SendProcessing.h"
#include "SendSender.h"
#include "XMLConfig.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#ifdef FLOW_TEST
#include "ImageFlowProcessing.h"
#endif

int main(char *args[], int count)
{
  #ifdef FLOW_TEST
  CvCapture *Capture = cvCreateCameraCapture(1);//0 - случайная камера, 1 - ps eye в случае ноутбука с вебкой
  cvSetCaptureProperty(Capture,CV_CAP_PROP_FRAME_WIDTH,320);
  cvSetCaptureProperty(Capture,CV_CAP_PROP_FRAME_HEIGHT,240);
  cvSetCaptureProperty(Capture,CV_CAP_PROP_FPS,180);//тут можно сменить параметры камеры
  ImageFlowProcessing *ImgFP = new ImageFlowProcessing();
  ImgFP->ShowOpticalFlow(Capture);//во время теста в директории с проектом возникают картинки вида Image[n].jpg, их надо удалять самому, так как у программы может не хватать прав на перезапись файлов
  cvReleaseCapture(&Capture);
  return 0;
  #endif

  XMLConfig config;
  { // Loading config from "config.xml" 
    std::ifstream ifs("config.xml");
	if (ifs.is_open()) {
	  boost::archive::xml_iarchive ia(ifs);
	  ia >> BOOST_SERIALIZATION_NVP(config);
	}
	else
	  cout << "Can't find config.xml! Default config used." << endl; // No config.xml found
  }

  KinectBuffer kinectBuffer1 (10);
  KinectBuffer kinectBuffer2 (10);
  KinectController kinectController (&kinectBuffer1);
  KinectDownsampler kinectDownsampler (&kinectBuffer1, &kinectBuffer2);
  KinectSender kinectSender (&config, &kinectBuffer2);
  ClientReceiver commandReceiver (&config);

  NanoReceivedBuffer NanoBuffer(1000);
  NanoController  NanoControl(&config, &NanoBuffer);

  ArduCopterBuffer CopterBuffer(1000);
  ArduCopterController CopterControl(&config, &CopterBuffer);

  CameraReceivedBuffer CameraBuffer(1000);
  CameraController CameraControl(&config, &CameraBuffer);

  SendBuffer sendBuffer (50);
  SendSender sendSender (&config, &sendBuffer);

  SendProcessing sendProcessing(&NanoBuffer, &CopterBuffer, &sendBuffer);
  

  boost::thread_group tgroup;

  tgroup.create_thread ( boost::bind (&KinectController::FakeStart, &kinectController) ); //don't have kinect so made FakeStart func for testing

  tgroup.create_thread ( boost::bind (&KinectDownsampler::Start, &kinectDownsampler) );

  tgroup.create_thread ( boost::bind (&KinectSender::Start, &kinectSender) );

  tgroup.create_thread ( boost::bind (&ClientReceiver::Start, &commandReceiver) );
  
  tgroup.create_thread ( boost::bind (&NanoController::Start, &NanoControl) );
  
  tgroup.create_thread ( boost::bind (&ArduCopterController::Start, &CopterControl) );

  tgroup.create_thread ( boost::bind (&CameraController::Start, &CameraControl) );

  tgroup.create_thread ( boost::bind (&SendProcessing::Start, &sendProcessing) );

  tgroup.create_thread ( boost::bind (&SendSender::Start, &sendSender) ); 

  #ifdef GPS_TEST
  char *UTC = new char(32);
  char *Lat = new char(32);
  char *Lon = new char(32);
  char *GSp = new char(32);
  char *GC = new char(32);
  while (true){
    int dUTC = 0;
    sprintf(UTC,"%d",dUTC);
    float dLat = 0.00053;
    sprintf(Lat,"000%.5f",dLat);
    float dLon = 0;
    sprintf(Lon,"0000%f",dLon);
    float dGSp = 0;
    sprintf(GSp,"%f",dGSp);
    float dGC = 0;
    sprintf(GC,"%f",dGC);
    NanoControl.ChangeGPSMessage(UTC,Lat,Lon,GSp,GC);
  }
  #endif

  tgroup.join_all ();
  

  return 0;
}