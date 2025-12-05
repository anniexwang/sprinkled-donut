#ifndef DONUT_H
#define DONUT_H

#include <Eigen/Core>

void donut(
  const int num_faces_u,
  const int num_faces_v,
  Eigen::MatrixXd & V,
  Eigen::MatrixXi & F,
  Eigen::MatrixXd & UV,
  Eigen::MatrixXi & UF,
  Eigen::MatrixXd & NV,
  Eigen::MatrixXi & NF,
  const double R,
  const double r);

#endif