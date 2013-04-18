#include "KinectDownsampler.h"

KinectDownsampler::KinectDownsampler (KinectBuffer * inputBuf, KinectBuffer * outputBuf)
{
  inputBuffer = inputBuf;
  outputBuffer = outputBuf;
}

void KinectDownsampler::Start(void)
{
  while (true) {
    boost::shared_ptr<KinectData> inputData;
    inputData = inputBuffer->Dequeue(); // Reading from input buffer

	if (!inputData->Cloud->empty() ) {
      boost::shared_ptr<KinectData> outputData (new KinectData); // Creating new KinectData

      outputData->Time = inputData->Time;
      downsample (inputData->Cloud, outputData->Cloud); // downsampling

      outputBuffer->Enqueue (outputData); // Writing to output buffer
	}
  }
}

KinectDownsampler::~KinectDownsampler()
{
}

void KinectDownsampler::downsample (const PointCloud<PointXYZ>::ConstPtr &CloudIn, PointCloud<PointXYZ>::Ptr &CloudOut)
{
  CloudOut->points.clear();
  CloudOut->is_dense = false;
  CloudOut->height = 48;
  CloudOut->width = 64;
  CloudOut->header = CloudIn->header;
  CloudOut->sensor_orientation_ = CloudIn->sensor_orientation_;
  CloudOut->sensor_origin_ = CloudIn->sensor_origin_;

  // We will take 1 point from every 10x10 block
  for (int i = 0; i < 640*480; i+=6400) { // Next line of 10x10 blocks 
    for (int j = i; j < i+640; j+=10) { // Next 10x10 block in line

	  // We want to take point with minimal z
      float min_value = CloudIn->points[j].z;
      int min_index = j;

      for (int k = j; k < j+6400; k+=640) { // Next line of points
        for (int l = k; l < k+10; l++) { // Next point
          if (min_value > CloudIn->points[l].z) {
            min_value = CloudIn->points[l].z;
            min_index = l;
          }
        }
      }
      CloudOut->points.push_back (CloudIn->points[min_index]); // Writing chosen point to output cloud

    }
  }

}