#include "sphere.h"
#include "donut.h"
#include <iostream>
#include <cmath>

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
  const double r)
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here:
  ////////////////////////////////////////////////////////////////////////////

  // same as sphere.cpp
  // num_faces_u * num_faces_v faces
  const int num_faces = num_faces_u * num_faces_v;
  
  // num_vertices_u = num_faces_u + 1
  // num_vertices_v = num_faces_v + 1
  const int num_vertices_u = num_faces_u + 1;
  const int num_vertices_v = num_faces_v + 1;
  const int num_vertices = num_vertices_u * num_vertices_v;

  // latitude - vertical
  // longitude - horizontal

  double pi = 2 * asin(1.0);

  // V.resize(num_vertices_u * num_vertices_v,3);
  // F.resize(num_faces_u * num_faces_v,4);
  // UV.resize(num_vertices_u * num_vertices_v,2);
  // UF.resize(num_faces_u * num_faces_v,4);
  // NV.resize(num_faces_u * num_faces_v,3);
  // NF.resize(num_faces_u * num_faces_v,4);

  // vertices
  V.resize(num_vertices,3);
  UV.resize(num_vertices,2);
  NV.resize(num_vertices,3);

  // fill up each row
  for (int i = 0; i < num_vertices_v; ++i) {
    // convert u and v to be [0, 1]
    double v = double(i) / num_faces_v;
    // phi = pi(v - 0.5)
    // double phi = pi * (v - 0.5);
    double phi = 2.0 * pi * v;

    for (int j = 0; j < num_vertices_u; ++j) {
      double u = double(j) / num_faces_u;

      int index = i * num_vertices_u + j;

      // add uv coordinates
      UV(index, 0) = u;
      UV(index, 1) = v;

      // theta = 2*pi*u
      double theta = 2 * pi * u;

      // different calculation for x, y, z of donut
      double x = (R + r * cos(phi))* cos(theta);
      double y = r * sin(phi);
      double z = (R + r * cos(phi)) * sin(theta);

      V(index, 0) = x;
      V(index, 1) = y;
      V(index, 2) = z;

      // normal of a donut is different
      NV(index, 0) = cos(phi) * cos(theta);
      NV(index, 1) = sin(phi);
      NV(index, 2) = cos(phi) * sin(theta);
    }
  }
  
  // faces
  F.resize(num_faces,4);
  UF.resize(num_faces,4);
  NF.resize(num_faces,4);
  for (int i = 0; i < num_faces_v; ++i) {
    for (int j = 0; j < num_faces_u; ++j) {
      int index = i * num_faces_u + j;

      int v0 = i * num_vertices_u + j;
      int v1 = v0 + 1;
      int v2 = v1 + num_vertices_u;
      int v3 = v2 - 1;

      F(index, 0) = v0;
      F(index, 1) = v3;
      F(index, 2) = v2;
      F(index, 3) = v1;
      UF(index, 0) = v0;
      UF(index, 1) = v3;
      UF(index, 2) = v2;
      UF(index, 3) = v1;
      NF(index, 0) = v0;
      NF(index, 1) = v3;
      NF(index, 2) = v2;
      NF(index, 3) = v1;
    }
  }
}
