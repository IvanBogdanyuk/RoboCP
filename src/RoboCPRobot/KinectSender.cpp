#include "KinectSender.h"

#include "KinectConfig.h"
#include "OctreeEncoderConfig.h"

KinectSender::KinectSender(KinectBuffer * buf)
{
  buffer = buf;
}

void KinectSender::Configure(Config* kinectConfig, Config* octreeConfig)
{

  KinectConfig* kinectCfg = (KinectConfig*) kinectConfig;
  OctreeEncoderConfig* octreeCfg = (OctreeEncoderConfig*) octreeConfig;

  port = kinectCfg->getPort(); //Reading port from config

  // We will encode point clouds before sending via octreeCoder
  // Parameters for constructor are taken from config
  octreeCoder = new PointCloudCompression<PointXYZ> (octreeCfg->getCompressionProfile(), octreeCfg->getShowStatistics(), octreeCfg->getPointResolution(),
                                                         octreeCfg->getOctreeResolution(), octreeCfg->getDoVoxelGridDownDownSampling(),
														 octreeCfg->getIFrameRate(), octreeCfg->getDoColorEncoding(), octreeCfg->getColorBitResolution());
}

void KinectSender::Start()
{
  try {
  boost::asio::io_service io_service;
	tcp::endpoint endpoint (tcp::v4 (), port);
  tcp::acceptor acceptor (io_service, endpoint);

  tcp::iostream socketStream;

  cout << "KinectSender: Waiting for connection.." << endl; //TODO: write in log
  #ifdef ENABLE_LOGGING
	RAW_LOG (INFO, "KinectSender: Waiting for connection..");
  #endif
  acceptor.accept (*socketStream.rdbuf ()); // waiting from connection from any IP

  cout << "KinectSender: Connected!" << endl; //TODO: write in log
  #ifdef ENABLE_LOGGING
	RAW_LOG (INFO, "KinectSender: Connected!");
  #endif

	while (!socketStream.fail() ) {
	  boost::shared_ptr<KinectData> pdata;
	  pdata = buffer->Dequeue(); // Read KinectData from buffer
	  socketStream << pdata->Time; // Send time
	  socketStream.flush();
	  octreeCoder->encodePointCloud (pdata->Cloud, socketStream); // Then send point cloud	
	}

  }
  catch (exception& e) {
    cout << "KinectSender: Exception: " << e.what () << endl; //TODO: write in log
    #ifdef ENABLE_LOGGING
	  RAW_LOG (INFO, "KinectSender: Exception: %s", e.what());
    #endif
  }

}

KinectSender::~KinectSender(void)
{
  delete (octreeCoder);
}
