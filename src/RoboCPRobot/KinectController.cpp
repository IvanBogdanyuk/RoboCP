#include "KinectController.h"

KinectController::KinectController(KinectBuffer * buf)
{
  buffer = buf;
}

void KinectController::grabberCallBack (const pcl::PointCloud<pcl::PointXYZ>::ConstPtr &cloud)
{
  PointCloud<PointXYZ>::Ptr cld (new PointCloud<PointXYZ> (*cloud) );
  boost::shared_ptr<KinectData> kData (new KinectData (cld, time(NULL) ) );
  buffer->Add (kData);
}

void KinectController::Start(void)
{
  Grabber* interface = new OpenNIGrabber ();

  boost::function<void
  (const PointCloud<PointXYZ>::ConstPtr&)> f = boost::bind (&KinectController::grabberCallBack, this, _1);

  boost::signals2::connection c = interface->registerCallback (f);

  interface->start ();
}

KinectController::~KinectController()
{
}

void KinectController::FakeStart ()
{
  cout << "KinectController: loading clouds..." << endl;
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud1 (new pcl::PointCloud<pcl::PointXYZ>);
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud2 (new pcl::PointCloud<pcl::PointXYZ>);
  pcl::io::loadPCDFile<pcl::PointXYZ> ("KinectCloud1.pcd", *cloud1 );
  pcl::io::loadPCDFile<pcl::PointXYZ> ("KinectCloud2.pcd", *cloud2 );
  //boost::shared_ptr<KinectData> kData1 (new KinectData (cloud1, 0) );
  //boost::shared_ptr<KinectData> kData2 (new KinectData (cloud2, 0) );
  cout << "KinectController: ready" << endl;

  while (true) {
	Sleep (2000);
	boost::shared_ptr<KinectData> kData1 (new KinectData (cloud1, time(NULL) ) );
	buffer->Add (kData1);

	Sleep (2000);
	boost::shared_ptr<KinectData> kData2 (new KinectData (cloud2, time(NULL) ) );
	buffer->Add (kData2);
  }
}