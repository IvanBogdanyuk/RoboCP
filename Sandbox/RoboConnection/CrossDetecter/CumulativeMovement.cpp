#include "CumulativeMovement.h"

inline float sqrdist(Point2f pt1, Point2f pt2)
{
  Point2f relative = pt1 - pt1;
  return relative.x*relative.x + relative.y*relative.y;
}

CumulativeMovement::CumulativeMovement(Point2f origin, Point2f target, int roi)
{
  mDistanceSqr = sqrdist(origin, target);
  mLastDist = mDistanceSqr;
  mLastPos = origin;
  mRoi = roi;
}

void CumulativeMovement::Accumulate(Point2f newpos)
{
  mDistanceSqr = sqrdist(newpos, mLastPos);
  mLastPos = newpos;
  if (mDistanceSqr > 10)
    if (mDistanceSqr > mLastDist)
      mSteerOffset += mDistanceSqr - mLastDist; // we are steering
    else
    {
      mMoveOffset += mLastDist - mDistanceSqr; // we are flying
    }
  else
    return;
}

CumulativeMovement::~CumulativeMovement()
{
}
