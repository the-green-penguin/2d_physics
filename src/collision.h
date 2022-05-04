/*

MIT License

Copyright (c) 2022 the_green_penguin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#pragma once

#include <memory>

#include <glm/glm.hpp>   // sudo apt install libglm-dev

#include "phy_object.h"



class Collision{
public:
  Collision(
    std::shared_ptr< PhyObject > phy_obj_0,
    std::shared_ptr< PhyObject > phy_obj_1
  );
  ~Collision();
  bool has_contact();
  void handle();
  
protected:
  struct edge{
    glm::vec2 p_0;
    glm::vec2 p_1;
  };
  struct projection{
    float min;
    float max;
  };
  
  bool contact = false;
  std::shared_ptr< PhyObject > phy_obj_0;
  std::shared_ptr< PhyObject > phy_obj_1;
  std::vector< glm::vec2 > points_0;
  std::vector< glm::vec2 > points_1;
  std::vector< glm::vec2 > surface_points_0;
  std::vector< glm::vec2 > surface_points_1;
  glm::vec2 center_0;
  glm::vec2 center_1;
  glm::vec2 velocity_0;
  glm::vec2 velocity_1;
  float ang_velocity_0;
  float ang_velocity_1;
  float impulse_0;
  float impulse_1;
  glm::vec2 coll_point;
  std::vector< edge > edges;
  
  void get_edges();
  void get_points();
  bool check_contact();
  bool check_contact_detailed();
  projection project_polygon(
    glm::vec2 axis,
    std::vector< glm::vec2 > polygon
  );
  bool check_proj_overlap(projection proj_0, projection proj_1);
  glm::vec2 perpendicular(glm::vec2 vec);
  void get_surface_points();
  void approximate_coll_point();
  void apply_impulse();
  void calc_impulse();
  glm::vec2 calc_velocity_at_coll_point();
  void get_velocities();
};