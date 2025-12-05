#include "per_corner_normals.h"
#include "triangle_area_normal.h"
// Hint:
#include "vertex_triangle_adjacency.h"
#include <iostream>
#include "per_face_normals.h"

void per_corner_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const double corner_threshold,
  Eigen::MatrixXd & N)
{
  N = Eigen::MatrixXd::Zero(F.rows()*3,3);
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here:
  ////////////////////////////////////////////////////////////////////////////

  double pi = 2 * asin(1.0);

  N.resize(F.rows() * 3, 3);

  std::vector<std::vector<int>> VF;
  vertex_triangle_adjacency(F, V.rows(), VF);
  
  Eigen::MatrixXd N_face;
  per_face_normals(V, F, N_face);

  for (int i = 0; i < F.rows(); ++i) {
    Eigen::RowVector3d face_normal = N_face.row(i);
    for (int j = 0; j < 3; ++j) {
    Eigen::RowVector3d sum = Eigen::RowVector3d::Zero();

    for (size_t g = 0; g < VF[F(i, j)].size(); ++g) {
      int index = VF[F(i, j)][g];
      Eigen::RowVector3d neighbour_face_normal = N_face.row(index);

      // calculate dot product
      double dot_product = face_normal.dot(neighbour_face_normal);
      
      // prevent undefined cos since -1 <= cos theta <= 1
      if (dot_product > 1.0) {
        dot_product = 1.0;
      }
      if (dot_product < -1.0) {
        dot_product = -1.0;
      }

      // a dot b = |a||b|cos(theta) -> theta = cos^-1 (a dot b)/(|a||b|)
      double theta = acos(dot_product / (face_normal.norm() * neighbour_face_normal.norm()));
      // convert to degrees
      theta = theta * 180.0 / pi;

      // if angle > threshold, dont average the normal
      if (theta < corner_threshold) {
        // N_face is from per_face_normals() which already calculates the triangle area normal
        sum += neighbour_face_normal;
      }
    }

    if (sum.norm() == 0) {
      N.row(i * 3 + j) = face_normal;
    }
    else {
      N.row(i * 3 + j) = sum.normalized();
    }
    }
  }
}
