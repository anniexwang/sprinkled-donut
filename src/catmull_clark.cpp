#include "catmull_clark.h"
#include <unordered_map>
#include <utility>
#include <functional>

void catmull_clark(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const int num_iters,
  Eigen::MatrixXd & SV,
  Eigen::MatrixXi & SF)
{
  // ////////////////////////////////////////////////////////////////////////////
  // // Replace with your code here:
  // SV = V;
  // SF = F;
  // ////////////////////////////////////////////////////////////////////////////

  SV = V;
  SF = F;

  for (int i = 0; i < num_iters; ++i) {
    Eigen::MatrixXd V_curr = SV;
    Eigen::MatrixXi F_curr = SF;
    // Step 1: Set the face point for each facet to be the average of its vertices
    std::vector<Eigen::RowVector3d> face_points(F_curr.rows());

    for (int f = 0; f < F_curr.rows(); ++f) {
      face_points[f] = (V_curr.row(F_curr(f,0)) + V_curr.row(F_curr(f,1)) + V_curr.row(F_curr(f,2)) + V_curr.row(F_curr(f,3))) / 4.0;
    } 

    // Step 2: Add edge points - average of the two neighbouring face points and edge end points
    std::vector<std::pair<int,int>> edge_points;

    for (int f = 0; f < F_curr.rows(); ++f) {
      // face vertices
      int v0 = F_curr(f, 0);
      int v1 = F_curr(f, 1);
      int v2 = F_curr(f, 2);
      int v3 = F_curr(f, 3);
      
      // edge end points
      auto e0_1 = std::make_pair(std::min(v0,v1), std::max(v0,v1));
      auto e1_2 = std::make_pair(std::min(v1,v2), std::max(v1,v2));
      auto e2_3 = std::make_pair(std::min(v2,v3), std::max(v2,v3));
      auto e3_0 = std::make_pair(std::min(v3,v0), std::max(v3,v0));
      
      edge_points.push_back(e0_1);
      edge_points.push_back(e1_2);
      edge_points.push_back(e2_3);
      edge_points.push_back(e3_0);
    }

    std::sort(edge_points.begin(), edge_points.end());
    edge_points.erase(std::unique(edge_points.begin(), edge_points.end()), edge_points.end());

    std::vector<std::vector<int>> neighbour_faces(edge_points.size());

    // edges of other faces
    for (int other = 0; other < F_curr.rows(); ++other) {
      int v_other0 = F_curr(other, 0);
      int v_other1 = F_curr(other, 1);
      int v_other2 = F_curr(other, 2);
      int v_other3 = F_curr(other, 3);

      // check if the edge of the other face match with the edge of our current face, if so, it is a neighbour face and we take that face's face point
      auto e_other0_1 = std::make_pair(std::min(v_other0,v_other1), std::max(v_other0,v_other1));
      auto e_other1_2 = std::make_pair(std::min(v_other1,v_other2), std::max(v_other1,v_other2));
      auto e_other2_3 = std::make_pair(std::min(v_other2,v_other3), std::max(v_other2,v_other3));
      auto e_other3_0 = std::make_pair(std::min(v_other3,v_other0), std::max(v_other3,v_other0));

      for (int i = 0; i < edge_points.size(); ++i) {
        if (e_other0_1 == edge_points[i] || e_other1_2 == edge_points[i] || e_other2_3 == edge_points[i] || e_other3_0 == edge_points[i]) {
          neighbour_faces[i].push_back(other);
        }
      }
    }

    // average of the 2 neighbouring face points and edge end points
    std::vector<Eigen::RowVector3d> edges(edge_points.size());
    
    for (int i = 0; i < edge_points.size(); ++i) {
      int v0 = edge_points[i].first;
      int v1 = edge_points[i].second;

      if (neighbour_faces[i].size() == 2) {
        edges[i] = (V_curr.row(v0) + V_curr.row(v1) + face_points[neighbour_faces[i][0]] + face_points[neighbour_faces[i][1]]) / 4.0;
      }
      else if (neighbour_faces[i].size() == 1) {
        edges[i] = (V_curr.row(v0) + V_curr.row(v1) + face_points[neighbour_faces[i][0]] + face_points[neighbour_faces[i][0]]) / 4.0;
      }
      else {
        edges[i] = (V_curr.row(v0) + V_curr.row(v1) + Eigen::RowVector3d::Zero() + Eigen::RowVector3d::Zero()) / 4.0;
      }
    }    
    
    // Step 4: Move each original vertex according to new position given by:
    // (F + 2R + (n - 3)P)/n
    // F: Average of all n created face points adjacent to P
    // R: Average of all original edge midpoints touching P
    std::vector<Eigen::RowVector3d> new_vertices;

    for (int v = 0; v < V_curr.rows(); ++v) {
      Eigen::RowVector3d P = V_curr.row(v);

      std::vector<int> adjacent_face_points;
      std::vector<int> adjacent_edge_points;
      Eigen::RowVector3d total_adjacent_face_points = Eigen::RowVector3d::Zero();
      Eigen::RowVector3d total_adjacent_edge_points = Eigen::RowVector3d::Zero();

      for (int f = 0; f < F_curr.rows(); ++f) {
        int v0 = F_curr(f, 0);
        int v1 = F_curr(f, 1);
        int v2 = F_curr(f, 2);
        int v3 = F_curr(f, 3);

        auto e0_1 = std::make_pair(std::min(v0,v1), std::max(v0,v1));
        auto e1_2 = std::make_pair(std::min(v1,v2), std::max(v1,v2));
        auto e2_3 = std::make_pair(std::min(v2,v3), std::max(v2,v3));
        auto e3_0 = std::make_pair(std::min(v3,v0), std::max(v3,v0));

        if ((v0 == v) || (v1 == v) || (v2 == v) || (v3 == v)) {
          adjacent_face_points.push_back(f);
          total_adjacent_face_points += face_points[f];
          int ei1;
          int ei2;
          if (v0 == v) {
            ei1 = std::find(edge_points.begin(), edge_points.end(), e0_1) - edge_points.begin();
            ei2 = std::find(edge_points.begin(), edge_points.end(), e3_0) - edge_points.begin();
          }
          if (v1 == v) {
            ei1 = std::find(edge_points.begin(), edge_points.end(), e1_2) - edge_points.begin();
            ei2 = std::find(edge_points.begin(), edge_points.end(), e0_1) - edge_points.begin();
          }
          if (v2 == v) {
            ei1 = std::find(edge_points.begin(), edge_points.end(), e2_3) - edge_points.begin();
            ei2 = std::find(edge_points.begin(), edge_points.end(), e1_2) - edge_points.begin();
          }
          if (v3 == v) {
            ei1 = std::find(edge_points.begin(), edge_points.end(), e3_0) - edge_points.begin();
            ei2 = std::find(edge_points.begin(), edge_points.end(), e2_3) - edge_points.begin();
          }

          adjacent_edge_points.push_back(ei1);
          adjacent_edge_points.push_back(ei2);
          total_adjacent_edge_points += edges[ei1];
          total_adjacent_edge_points += edges[ei2];
        }
      }

      int n = adjacent_face_points.size();

      if ((n == 0) || (adjacent_edge_points.size() == 0)) {
        new_vertices.push_back(P);
        continue;
      }

      Eigen::RowVector3d F = total_adjacent_face_points / n;
      Eigen::RowVector3d R = total_adjacent_edge_points / adjacent_edge_points.size();

      Eigen::RowVector3d new_position = (F + 2 * R + (n - 3) * P) / n;
      new_vertices.push_back(new_position);
    }

    // Step 5: Connect up original points to make facets
    // assign SV
    int SV_index = 0;
    SV = Eigen::MatrixXd(new_vertices.size() + face_points.size() + edges.size(), 3);
    for (int v = 0; v < new_vertices.size(); ++v) {
      SV.row(SV_index) = new_vertices[v];
      SV_index++;
    }
    for (int f = 0; f < face_points.size(); ++f) {
      SV.row(SV_index) = face_points[f];
      SV_index++;
    }
    for (int e = 0; e < edges.size(); ++e) {
      SV.row(SV_index) = edges[e];
      SV_index++;
    }

    // create facets (they need to match the index in SV)
    std::vector<Eigen::RowVector4i> facets;

    for (int f = 0; f < F_curr.rows(); ++f) {
      int v0 = F_curr(f, 0);
      int v1 = F_curr(f, 1);
      int v2 = F_curr(f, 2);
      int v3 = F_curr(f, 3);

      auto e0_1 = std::make_pair(std::min(v0,v1), std::max(v0,v1));
      auto e1_2 = std::make_pair(std::min(v1,v2), std::max(v1,v2));
      auto e2_3 = std::make_pair(std::min(v2,v3), std::max(v2,v3));
      auto e3_0 = std::make_pair(std::min(v3,v0), std::max(v3,v0));

      int ei0 = (std::find(edge_points.begin(), edge_points.end(), e0_1) - edge_points.begin());
      int ei1 = (std::find(edge_points.begin(), edge_points.end(), e1_2) - edge_points.begin());
      int ei2 = (std::find(edge_points.begin(), edge_points.end(), e2_3) - edge_points.begin());
      int ei3 = (std::find(edge_points.begin(), edge_points.end(), e3_0) - edge_points.begin());

      facets.push_back(Eigen::RowVector4i(v0, new_vertices.size() + face_points.size() + ei0, new_vertices.size() + f, new_vertices.size() + face_points.size() + ei3));
      facets.push_back(Eigen::RowVector4i(v1, new_vertices.size() + face_points.size() + ei1, new_vertices.size() + f, new_vertices.size() + face_points.size() + ei0));
      facets.push_back(Eigen::RowVector4i(v2, new_vertices.size() + face_points.size() + ei2, new_vertices.size() + f, new_vertices.size() + face_points.size() + ei1));
      facets.push_back(Eigen::RowVector4i(v3, new_vertices.size() + face_points.size() + ei3, new_vertices.size() + f, new_vertices.size() + face_points.size() + ei2));
    }
    // set SF = facets
    SF.resize(facets.size(), 4);
    for (int i = 0; i < facets.size(); ++i) {
      SF.row(i) = facets[i];
    }
  }
}