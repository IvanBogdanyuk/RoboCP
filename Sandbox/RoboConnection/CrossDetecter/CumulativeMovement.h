#pragma once

#include <opencv2/core/core.hpp>
using namespace cv;

class CumulativeMovement
{
public:
  CumulativeMovement::CumulativeMovement(Point2f origin, Point2f target);
  ~CumulativeMovement();
  void CumulativeMovement::Accumulate(Point2f newpos);
private:
  Point2f mMovement;
  Point2f mLastPos;
  float mLastDist;
  float mDistanceSqr;
  float mSteerOffset = 0;
  float mMoveOffset = 0;
};

