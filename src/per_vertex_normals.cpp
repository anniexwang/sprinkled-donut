#include "per_vertex_normals.h"
#include "triangle_area_normal.h"
#include "vertex_triangle_adjacency.h"

void per_vertex_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  Eigen::MatrixXd & N)
{
  // N = Eigen::MatrixXd::Zero(V.rows(),3);
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here:
  ////////////////////////////////////////////////////////////////////////////

  N.resize(V.rows(), 3);

  std::vector<std::vector<int>> VF;
  vertex_triangle_adjacency(F, V.rows(), VF);

  for (size_t v = 0; v < VF.size(); ++v) {
    Eigen::RowVector3d sum = Eigen::RowVector3d::Zero();

    for (size_t f = 0; f < VF[v].size(); ++f) {
      int index = VF[v][f];

      Eigen::RowVector3d a = V.row(F(index, 0));
      Eigen::RowVector3d b = V.row(F(index, 1));
      Eigen::RowVector3d c = V.row(F(index, 2));

      sum += triangle_area_normal(a, b, c);
    }

    N.row(v) = sum.normalized();
  }
}
