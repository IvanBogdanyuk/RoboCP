#include "KinectSender.h"


KinectSender::KinectSender(XMLConfig * x, KinectBuffer * buf)
{
  port = atoi(x->KinectPort.c_str() );
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

    cout << "KinectSender: Waiting for connection.." << endl;

    acceptor.accept (*socketStream.rdbuf ());

    cout << "KinectSender: Connected!" << endl;

	

	while (!socketStream.fail() ) {
	  boost::shared_ptr<KinectData> pdata;
	  pdata = buffer->Rec();
	  socketStream << pdata->Time;
	  socketStream.flush();
	  octreeCoder->encodePointCloud (pdata->Cloud, socketStream);	
	}

  }
  catch (exception& e) {
    cout << "KinectSender: Exception: " << e.what () << endl;
  }

}

KinectSender::~KinectSender(void)
{
  delete (octreeCoder);
}
