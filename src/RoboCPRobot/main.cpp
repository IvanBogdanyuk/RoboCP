#pragma once

//#define FLOW_TEST
//#define GPS_TEST
//#define MOTION_TEST

#include <stdarg.h>

#include "KinectController.h"
#include "KinectDownsampler.h"
#include "KinectSender.h"
#include "NanoController.h"
#include "ArduCopterController.h"
#include "CameraController.h"
#include "CommandProcessing.h"
#include "ClientReceiver.h"
#include "SendProcessing.h"
#include "SendSender.h"
#include "Config.h"
#include "configFactory.h"

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#ifdef ENABLE_LOGGING
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include <glog/raw_logging.h>
#endif

#ifdef FLOW_TEST
#include "ImageFlowProcessing.h"
#endif
#ifdef MOTION_TEST
#include "ImageFlowProcessing.h"
#endif

int main(char *args[], int count)
{
	freopen ("log.log", "a", stderr);
  #ifdef ENABLE_LOGGING
	RAW_LOG (INFO, "MAIN ROBOT STARTED");
  #endif

  #ifdef MOTION_TEST
    CvCapture *capture = cvCreateCameraCapture(0);//0 - ñëó÷àéíàÿ êàìåðà, 1 - ps eye â ñëó÷àå íîóòáóêà ñ âåáêîé
    cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH,320);
    cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT,240);
    cvSetCaptureProperty(capture,CV_CAP_PROP_FPS,180);//òóò ìîæíî ñìåíèòü ïàðàìåòðû êàìåðû
  
    CvCapture *capture2 = cvCreateCameraCapture(0);//0 - ñëó÷àéíàÿ êàìåðà, 1 - ps eye â ñëó÷àå íîóòáóêà ñ âåáêîé
    cvSetCaptureProperty(capture2,CV_CAP_PROP_FRAME_WIDTH,320);
    cvSetCaptureProperty(capture2,CV_CAP_PROP_FRAME_HEIGHT,240);
    cvSetCaptureProperty(capture2,CV_CAP_PROP_FPS,180);//òóò ìîæíî ñìåíèòü ïàðàìåòðû êàìåðû
  
	if(capture != NULL){
		ImageFlowProcessing obj;
		cvSetCaptureProperty( capture2, CV_CAP_PROP_POS_FRAMES, 50 );		
		IplImage *Img1 = 0, *Img2= 0;
		Img1 = cvQueryFrame( capture2 );
	//	cvSetCaptureProperty( capture, CV_CAP_PROP_POS_FRAMES, 200 );
		Img2 = cvQueryFrame( capture );		
		DisplacementImages res;
		obj.CountDisplacement(Img1, Img2, &res);
		
		for(int i=0; i< res.NumVectors; i++){
			cvLine( Img1, cvPoint(res.Vectors[i].Beginning.x, res.Vectors[i].Beginning.y), cvPoint(res.Vectors[i].End.x, res.Vectors[i].End.y), CV_RGB(0,255,0),1);
		}
		res.CountMotion();
		printf("\nMotion: (%.2f  %.2f) - (%.2f  %.2f)  Length == %.2f\n",res.Motion.Beginning.x, res.Motion.Beginning.y, res.Motion.End.x, res.Motion.End.y, res.Motion.Length);
		cvLine( Img1, cvPoint(res.Motion.Beginning.x, res.Motion.Beginning.y), cvPoint(res.Motion.End.x, res.Motion.End.y), CV_RGB(255,0,0),4);
		cvShowImage("Image1", Img1);
		cvShowImage("Image2", Img2);
		int key_pressed;
		key_pressed = cvWaitKey(0);
		res.~DisplacementImages(); 
	}
	else{
		printf("Not Found\n");
		scanf("%d");
	}	
  #endif

  #ifdef FLOW_TEST
  CvCapture *Capture = cvCreateCameraCapture(0);//0 - ñëó÷àéíàÿ êàìåðà, 1 - ps eye â ñëó÷àå íîóòáóêà ñ âåáêîé
  cvSetCaptureProperty(Capture,CV_CAP_PROP_FRAME_WIDTH,320);
  cvSetCaptureProperty(Capture,CV_CAP_PROP_FRAME_HEIGHT,240);
  cvSetCaptureProperty(Capture,CV_CAP_PROP_FPS,180);//òóò ìîæíî ñìåíèòü ïàðàìåòðû êàìåðû
  ImageFlowProcessing *ImgFP = new ImageFlowProcessing();
  ImgFP->ShowOpticalFlow(Capture);//âî âðåìÿ òåñòà â äèðåêòîðèè ñ ïðîåêòîì âîçíèêàþò êàðòèíêè âèäà Image[n].jpg, èõ íàäî óäàëÿòü ñàìîìó, òàê êàê ó ïðîãðàììû ìîæåò íå õâàòàòü ïðàâ íà ïåðåçàïèñü ôàéëîâ
  cvReleaseCapture(&Capture);
  return 0;
  #endif

  /*
  XMLConfig config;
  { // Loading config from "config.xml" 
    std::ifstream ifs("config.xml");
	if (ifs.is_open()) {
	  boost::archive::xml_iarchive ia(ifs);
	  ia >> BOOST_SERIALIZATION_NVP(config);
	}
	else
	  cout << "Can't find config.xml! Default config used." << endl; // No config.xml found
  }	*/

  configFactory cfgFactory;
  cfgFactory.Parse();

  KinectBuffer kinectBuffer1 (10);
  KinectBuffer kinectBuffer2 (10);
  KinectController kinectController (&kinectBuffer1);
  KinectDownsampler kinectDownsampler (&kinectBuffer1, &kinectBuffer2);

  KinectSender kinectSender (&kinectBuffer2);
  kinectSender.Configure(cfgFactory.ConfigByName("Kinect"), cfgFactory.ConfigByName("OctreeEncoder"));

  ClientReceiver commandReceiver;
  commandReceiver.Configure(cfgFactory.ConfigByName("Command"));

  NanoReceivedBuffer NanoBuffer(1000);
  NanoController  NanoControl(&NanoBuffer);
  NanoControl.Configure(cfgFactory.ConfigByName("Carduino"));

  ArduCopterBuffer CopterBuffer(1000);
  ArduCopterController CopterControl(&CopterBuffer);
  CopterControl.Configure(cfgFactory.ConfigByName("Arducopter"));

  CameraReceivedBuffer CameraBuffer(1000);
  CameraController CameraControl(&CameraBuffer);
  CameraControl.Configure(cfgFactory.ConfigByName("Camera"));

  CommandBuffer ComBuffer(100);
  CommandProcessing ComProc(&ComBuffer);

  SendBuffer sendBuffer (50);
  SendSender sendSender (&sendBuffer);
  sendSender.Configure(cfgFactory.ConfigByName("Send"));

  SendProcessing sendProcessing(&NanoBuffer, &CopterBuffer, &CameraBuffer, &sendBuffer);
  

  boost::thread_group tgroup;

  tgroup.create_thread ( boost::bind (&KinectController::FakeStart, &kinectController) ); //don't have kinect so made FakeStart func for testing

  tgroup.create_thread ( boost::bind (&KinectDownsampler::Start, &kinectDownsampler) );

  tgroup.create_thread ( boost::bind (&KinectSender::Start, &kinectSender) );

  tgroup.create_thread ( boost::bind (&ClientReceiver::Start, &commandReceiver) );
  
  tgroup.create_thread ( boost::bind (&NanoController::Start, &NanoControl) );
  
  tgroup.create_thread ( boost::bind (&ArduCopterController::Start, &CopterControl) );

  tgroup.create_thread ( boost::bind (&CameraController::Start, &CameraControl) );

  tgroup.create_thread ( boost::bind (&CommandProcessing::Start, &ComProc) );

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

