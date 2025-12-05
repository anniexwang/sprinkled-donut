#include "write_obj.h"
#include <fstream>
#include <cassert>
#include <iostream>

bool write_obj(
  const std::string & filename,
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const Eigen::MatrixXd & UV,
  const Eigen::MatrixXi & UF,
  const Eigen::MatrixXd & NV,
  const Eigen::MatrixXi & NF)
{
  assert((F.size() == 0 || F.cols() == 3 || F.cols() == 4) && "F must have 3 or 4 columns");
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here:
  ////////////////////////////////////////////////////////////////////////////
  // return false;

  /*
   # a comment line - what program wrote the file out
   v x y z - specifies vertex by its 3 coordinates
   vt u v [w] - vertex texture specifies uw and optionally w - 0-1 values
   vn x y z - vertex normal
   g name - group name
   usemtl name - name a material to use
   f v1[/vt1][/vn1] v2[/vt2][/vn2] v3[/vt3][/vn3] ...

   vt = UV
   vn = NV
   vt_i = UF
   vn_i = NF
  */

  std::ofstream ofs(filename);
  if (!ofs) return false;

  ofs << "# .obj written by write_obj\n";

  for (int i = 0; i < V.rows(); ++i) {
    ofs << "v " << V(i, 0) << " " << V(i, 1) << " " << V(i, 2) << "\n";
  }

  for (int i = 0; i < UV.rows(); ++i) {
    ofs << "vt " << UV(i, 0) << " " << UV(i, 1) << "\n";
  }

  for (int i = 0; i < NV.rows(); ++i) {
    ofs << "vn " << NV(i, 0) << " " << NV(i, 1) << " " << NV(i, 2) << "\n";
  }

  for (int i = 0; i < F.rows(); ++i) {
    ofs << "f";
    for (int j = 0; j < F.cols(); ++j) {
      ofs << " " << F(i, j) + 1;
      if ((UV.rows() > 0 && UF.rows() > 0) || (NV.rows() > 0 && NF.rows() > 0)) {
        ofs << "/";
        if ((UV.rows() > 0 && UF.rows() > 0)) {
          ofs << UF(i, j) + 1;
        }
        if ((NV.rows() > 0 && NF.rows() > 0)) {
          ofs << "/" << NF(i, j) + 1;
        }
      }
    }
    ofs << "\n";
  }

  return ofs.good();
}
