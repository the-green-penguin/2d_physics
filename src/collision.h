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
  Collision(
    std::shared_ptr< PhyObject > phy_obj_0,
    std::shared_ptr< PhyObject > phy_obj_1,
    id window_id
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
  bool visible = false;
  id window_id;
  id collision_marker;
  glm::vec2 ref_pos;
  float ref_rot;
  glm::vec2 coll_point;
  glm::vec2 rel_coll_point_0;
  glm::vec2 rel_coll_point_1;
  glm::vec2 coll_normal;
  glm::vec2 impact_velocity;
  
  bool check_contact();
  bool check_contact_detailed();
  projection project_polygon(
    glm::vec2 axis,
    std::vector< glm::vec2 > polygon
  );
  bool check_proj_overlap(projection proj_0, projection proj_1);
  glm::vec2 perpendicular(glm::vec2 vec);
  void apply_impulse();
  float calc_impulse();
  void fetch_collision_variables();
  glm::vec2 approximate_coll_point(
    const std::vector< glm::vec2 >& points_0,
    const std::vector< glm::vec2 >& points_1
  );
  glm::vec2 approx_rel_coll_point(
    const std::vector< glm::vec2 >& surface_points,
    glm::vec2 opposite_center
  );
  glm::vec2 refine_nearest_point(
    glm::vec2 curr_point,
    glm::vec2 neighbour_0,
    glm::vec2 neighbour_1,
    glm::vec2 target,
    int max_depth
  );
  glm::vec2 calc_impact_velocity();
  std::vector< glm::vec2 > fetch_points_world_space(   // does the compiler really optimise return by value for std types?
    std::shared_ptr< PhyObject > phy_obj
  );
  std::vector< edge > fetch_edges(   // does the compiler really optimise return by value for std types?
    const std::vector< glm::vec2 >& points_0,
    const std::vector< glm::vec2 >& points_1
  );
  static void to_world_space(
    std::vector< glm::vec2 >& points,
    glm::vec2 offset,
    float rotation
  );
  static void to_world_space(
    glm::vec2& point,
    glm::vec2 offset,
    float rotation
  );
  static void to_object_space(
    std::vector< glm::vec2 >& points,
    glm::vec2 offset,
    float rotation
  );
  static void to_object_space(
    glm::vec2& point,
    glm::vec2 offset,
    float rotation
  );
  static float cross_2d(glm::vec2 v_0, glm::vec2 v_1);
};