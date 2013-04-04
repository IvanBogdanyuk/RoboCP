#include "KinectController.h"

KinectController::KinectController(KinectBuffer * buf)
{
  buffer = buf;
}

void KinectController::grabberCallBack (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr &cloud)
{
  boost::shared_ptr<KinectData> kData (new KinectData);
  
  *(kData->Cloud) += *cloud; // add KinectDownSampler class, and do something like KDS.DownSample (kData->Cloud, cloud)

  buffer->Add (kData);
}

void KinectController::Start(void)
{
  Grabber* interface = new OpenNIGrabber ();

  boost::function<void
  (const PointCloud<PointXYZRGBA>::ConstPtr&)> f = boost::bind (&KinectController::grabberCallBack, this, _1);

  boost::signals2::connection c = interface->registerCallback (f);

  interface->start ();
}

KinectController::~KinectController()
{
}

void KinectController::FakeStart ()
{
  while (true) {
	Sleep (2000);
	boost::shared_ptr<KinectData> kdata1 (new KinectData);
	pcl::io::loadPCDFile<pcl::PointXYZRGBA> ("kitchen00001.pcd", *(kdata1->Cloud) );
	buffer->Add (kdata1);

	Sleep (2000);
	boost::shared_ptr<KinectData> kdata2 (new KinectData);
	pcl::io::loadPCDFile<pcl::PointXYZRGBA> ("table.pcd", *(kdata2->Cloud) );
	buffer->Add (kdata2);
  }
}