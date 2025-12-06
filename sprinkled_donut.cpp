#include "donut.h"
#include "sphere.h"
#include "write_obj.h"
#include "QuadViewer.h"
#include "set_texture_from_png.h"
#include <igl/readOBJ.h>
#include <Eigen/Core>
#include <vector>
#include <iostream>
#include <memory>
#include <limits>
#include <functional>
#include <algorithm>
#include <cmath>


int main(int argc, char * argv[])
{
    double SCALE = 0.02;
    // Create mesh of a donut
    Eigen::MatrixXd V_donut, UV_donut, NV_donut;
    Eigen::MatrixXi F_donut, UF_donut, NF_donut;
    const double R = 1.5;
    const double r = 0.7;
    donut(60, 40, V_donut, F_donut, UV_donut, UF_donut, NV_donut, NF_donut, R, r);
    V_donut *= SCALE;

    // Create mesh of a sphere
    Eigen::MatrixXd V_sprinkle, UV_sprinkle, NV_sprinkle;
    Eigen::MatrixXi F_sprinkle, UF_sprinkle, NF_sprinkle;
    sphere(20 ,20, V_sprinkle, F_sprinkle, UV_sprinkle, UF_sprinkle, NV_sprinkle, NF_sprinkle);

    int num_sprinkles = 100;

    std::vector<Eigen::MatrixXd> V_sprinkles(num_sprinkles);
    std::vector<Eigen::Vector3d> position(num_sprinkles);
    std::vector<Eigen::Vector3d> velocity(num_sprinkles);

    // FPS
    double delta_t = 0.016;
    // Gravity
    double g = -9.8;
    
    // Possible sprinkle colors
    std::vector<Eigen::Vector3d> colors = {
        Eigen::Vector3d(1.0, 0.0, 0.0), // red
        Eigen::Vector3d(0.0, 1.0, 0.0), // green
        Eigen::Vector3d(0.0, 0.0, 1.0), // blue
        Eigen::Vector3d(1.0, 1.0, 0.0), // yellow
        Eigen::Vector3d(1.0, 1.0, 1.0), // white
        Eigen::Vector3d(1.0, 0.3, 0.8)  // pink
    };

    std::vector<Eigen::MatrixXd> sprinkle_color(num_sprinkles);

    // Set position, velocity, and color for each sprinkle
    for (int i = 0; i < num_sprinkles; i++) {
        double theta = ((double) rand() / RAND_MAX) * 2 * M_PI; 
        // Make sure sprinkles are only on top of the donut and not in the hole (pi/4 to 3pi/4)
        double phi = M_PI * (0.25 + 0.5 * ((double) rand() / RAND_MAX));
        
        double x = (R + r * cos(phi)) * cos(theta);
        double y = r * sin(phi);
        double z = (R + r * cos(phi)) * sin(theta);

        position[i] = SCALE * Eigen::Vector3d(x, y, z) + Eigen::Vector3d(0, 0.05, 0); // start above donut
        velocity[i] = Eigen::Vector3d(0, 0, 0);

        V_sprinkles[i] = V_sprinkle * 0.05;

        Eigen::Vector3d color = colors[rand() % colors.size()];
        sprinkle_color[i].resize(V_sprinkle.rows(), 3);
        for (int v = 0; v < V_sprinkle.rows(); v++) {
            sprinkle_color[i].row(v) = color.transpose();
        }
    }
    
    {
        QuadViewer v;
        // Donut
        v.set_mesh(V_donut, F_donut);
        set_texture_from_png("../data/glazed_donut.png", v.viewer.data());
        v.viewer.data().show_lines = false;
        v.viewer.data().show_overlay = false;
        v.viewer.data().line_width = 0.0;

        // Sprinkles
        for (int i = 0; i < num_sprinkles; i++) {
            v.viewer.append_mesh();
            int index = v.viewer.data_list.size() - 1;

            v.viewer.data_list[index].set_mesh(V_sprinkles[i], F_sprinkle);
            
            // Get rid of grid
            v.viewer.data().show_lines = false;
            v.viewer.data().show_overlay = false;
            v.viewer.data().line_width = 0.0;

            v.viewer.data_list[index].set_face_based(true);
            v.viewer.data_list[index].set_colors(sprinkle_color[i].colwise().mean().replicate(F_sprinkle.rows(), 1));
        }

        // Create falling animation
        v.viewer.callback_pre_draw = [&](igl::opengl::glfw::Viewer &viewer)
        {
            for (int i = 0; i < num_sprinkles; i++)
            {
                // Apply gravity
                velocity[i].y() += g * delta_t;

                // Update position
                position[i] += velocity[i] * delta_t;

                // Find the surface for the top of the donut
                double xz_radius = std::sqrt(position[i].x() * position[i].x() + position[i].z() * position[i].z());
                double phi_surface = std::acos((xz_radius / SCALE - R) / r);
                double y_surface = r * std::sin(phi_surface) * SCALE;

                // Stop the animation when it hits the surface
                if (position[i].y() < y_surface)
                {
                    position[i].y() = y_surface;
                    velocity[i].y() = 0;
                }

                // Move sprinkle vertices to follow updated position
                for (int v = 0; v < V_sprinkle.rows(); v++)
                {
                    V_sprinkles[i].row(v) =
                        V_sprinkle.row(v) * (0.05 * SCALE) +
                        position[i].transpose();
                }

                // Replace sprinkle mesh in viewer
                int mesh_index = 1 + i; // donut = 0, sprinkles start at 1
                viewer.data_list[mesh_index].set_vertices(V_sprinkles[i]);
            }
            return false;
        };
        v.launch();
    }
    return 0;
}
