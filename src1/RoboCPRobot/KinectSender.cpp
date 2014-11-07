#include "KinectSender.h"


KinectSender::KinectSender(XMLConfig * x, KinectBuffer * buf)
{
  port = atoi(x->KinectPort.c_str() ); //Reading port from config

  // We will encode point clouds before sending via octreeCoder
  // Parameters for constructor are taken from config
  octreeCoder = new PointCloudCompression<PointXYZ> (x->CompressionProfile, x->ShowStatistics, x->PointResolution,
                                                         x->OctreeResolution, x->DoVoxelGridDownDownSampling, x->IFrameRate,
                                                         x->DoColorEncoding, x->ColorBitResolution);
  buffer = buf;
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
