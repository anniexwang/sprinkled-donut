#include "triangle_area_normal.h"
#include <Eigen/Geometry>
#include <cmath>

Eigen::RowVector3d triangle_area_normal(
  const Eigen::RowVector3d & a, 
  const Eigen::RowVector3d & b, 
  const Eigen::RowVector3d & c)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code:
  ////////////////////////////////////////////////////////////////////////////
  // return Eigen::RowVector3d(0,0,0);

  // (p1 - p0) x (p2 - p0) = 2A dot n
  // area normal = 2A dot n
  // p0 = a
  // p1 = b
  // p2 = c

  return Eigen::RowVector3d((b - a).cross((c - a))).transpose();
}
