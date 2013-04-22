#pragma once
#include "SendReceiver.h"


SendReceiver::SendReceiver (XMLConfig * x, KinectViewer* v)
{
  ip = x->IP; // Reading IP from config
  port = x->SendPort; // Reading port from config
  kinectViewer = v;
}

SendReceiver::~SendReceiver ()
{
}

void SendReceiver::Start ()
{
  try {
    tcp::iostream socketStream (ip.c_str(), port.c_str() ); // Trying to connect

    if (!socketStream.fail() ) {
      cout << "SendReceiver: Connected!" << endl; // TODO: write in log
	  RAW_LOG (INFO, "SendReceiver: Connected!");

	  boost::archive::xml_iarchive ia(socketStream); // We will receive Send objects in XML
	  boost::shared_ptr<Send> sendData (new Send);  // Creating new Send object
	  Sleep(5000);

	  while ( true ) {
		ia >> BOOST_SERIALIZATION_NVP(sendData); // Receiving
		updateText(sendData); // Updating KinectViewer info
	  }
	
	}

  }
  catch (exception& e) {
    cout << "KinectViewer: Exception: " << e.what () << endl; // TODO: write in log
	RAW_LOG (INFO, "KinectViewer: Exception: %s", e.what());
  }
}

void SendReceiver::updateText (boost::shared_ptr<Send> s)
{
  char buf[40];

  // Filling buf with sprintf, then updating text in KinectViewer 

  sprintf (buf, "Acceleration x: %d", s->Acceleration.x);
  kinectViewer->viewer->updateText (buf, 5, 290, 10, 1, 1, 1, "AccelerationX");
  sprintf (buf, "Acceleration y: %d", s->Acceleration.y);
  kinectViewer->viewer->updateText (buf, 5, 278, 10, 1, 1, 1, "AccelerationY");
  sprintf (buf, "Acceleration z: %d", s->Acceleration.z);
  kinectViewer->viewer->updateText (buf, 5, 268, 10, 1, 1, 1, "AccelerationZ");
  
  sprintf (buf, "Top sonic: %d", s->TopSonicSensor);
  kinectViewer->viewer->updateText (buf, 5, 248, 10, 1, 1, 1, "TopSonic");
  sprintf (buf, "Left sonic: %d", s->LeftSonicSensor);
  kinectViewer->viewer->updateText (buf, 5, 238, 10, 1, 1, 1, "LeftSonic");
  sprintf (buf, "Right sonic: %d", s->RightSonicSensor);
  kinectViewer->viewer->updateText (buf, 5, 224, 10, 1, 1, 1, "RightSonic");
  sprintf (buf, "Front sonic: %d", s->FrontSonicSensor);
  kinectViewer->viewer->updateText (buf, 5, 214, 10, 1, 1, 1, "FrontSonic");
  sprintf (buf, "Back sonic: %d", s->BackSonicSensor);
  kinectViewer->viewer->updateText (buf, 5, 202, 10, 1, 1, 1, "BackSonic");

  sprintf (buf, "Alt barometer: %f", s->AltitudeBarometer);
  kinectViewer->viewer->updateText (buf, 5, 186, 10, 1, 1, 1, "AltBarometer");
  sprintf (buf, "Alt sonic: %f", s->AltitudeSonic);
  kinectViewer->viewer->updateText (buf, 5, 174, 10, 1, 1, 1, "AltSonic");
  sprintf (buf, "Pitch: %f", s->Pitch);
  kinectViewer->viewer->updateText (buf, 5, 162, 10, 1, 1, 1, "Pitch");
  sprintf (buf, "Roll: %f", s->Roll);
  kinectViewer->viewer->updateText (buf, 5, 150, 10, 1, 1, 1, "Roll");
  sprintf (buf, "Yaw: %f", s->Yaw);
  kinectViewer->viewer->updateText (buf, 5, 138, 10, 1, 1, 1, "Yaw");

  sprintf (buf, "Time: %s", ctime(&(s->Time)) );
  kinectViewer->viewer->updateText (buf, 5, 122, 10, 1, 1, 1, "SendTime");
  
}
