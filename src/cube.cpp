#include "cube.h"

void cube(
  Eigen::MatrixXd & V,
  Eigen::MatrixXi & F,
  Eigen::MatrixXd & UV,
  Eigen::MatrixXi & UF,
  Eigen::MatrixXd & NV,
  Eigen::MatrixXi & NF)
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here:
  ////////////////////////////////////////////////////////////////////////////

  ////Hint:
  // V.resize(8,3);
  // F.resize(6,4);
  // UV.resize(14,2);
  // UF.resize(6,4);
  // NV.resize(6,3);
  // NF.resize(6,4);


  V.resize(8,3);

  V << 0, 0, 0, // v0
       1, 0, 0, // v1
       1, 1, 0, // v2
       0, 1, 0, // v3
       0, 0, 1, // v4
       1, 0, 1, // v5
       1, 1, 1, // v6
       0, 1, 1; // v7

  F.resize(6,4);

  F << 0, 1, 2, 3, // bottom 
       4, 5, 6, 7, // top
       1, 2, 6, 5, // front
       0, 3, 7, 4, // back
       0, 1, 5, 4, // left
       2, 3, 7, 6; // right
  
  UV.resize(14,2);

  UV << 1-0.25, 0, // uv0
        1-0.5, 0, // uv1
        1-0.25, 0.25, // uv2
        1-0.5, 0.25, // uv3
        1-0.25, 0.5, // uv4
        1-0.5, 0.5, // uv5
        1-0.25, 0.75, // uv6
        1-0.5, 0.75, // uv7
        1-0, 0.25, // uv8
        1-0, 0.5, // uv9
        1-0.75, 0.25, // uv10
        1-0.75, 0.5, // uv11
        1-1, 0.25, // uv12
        1-1, 0.5; // uv13

  UF.resize(6,4);

  UF << 11, 5, 3, 10, // bottom
        9, 4, 2, 8, // top
        5, 3, 2, 4, // front
        11, 10, 12, 13, // back
        7, 5, 4, 6, // left
        3, 1, 0, 2; // right

  NV.resize(6,3);

  NV << 0, 0, -1, // bottom normal - green
        0, 0, 1, // top normal - orange
        1, 0, 0, // front normal - blue
        -1, 0, 0, // back normal - red
        0, -1, 0, // left normal
        0, 1, 0; // right normal


  NF.resize(6,4);

  NF << 0, 0, 0, 0, // bottom
        1, 1, 1, 1, // top
        2, 2, 2, 2, // front
        3, 3, 3, 3, // back
        4, 4, 4, 4, // left
        5, 5, 5, 5; // right
}