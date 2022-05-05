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

#include "collision.h"

#include <iostream>
#include <vector>



Collision::Collision(std::shared_ptr< PhyObject > phy_obj_0, std::shared_ptr< PhyObject > phy_obj_1){
  this->phy_obj_0 = phy_obj_0;
  this->phy_obj_1 = phy_obj_1;
  contact = check_contact();
}



//------------------------------------------------------------------------------
Collision::~Collision(){}



//------------------------------------------------------------------------------
bool Collision::has_contact(){  return contact;  }



//------------------------------------------------------------------------------
void Collision::handle(){
  if( ! contact) return;   // skip if there is no contact
  
  std::cout << "!" << std::flush;
  
  
}



////////////////////////////////////////////////////////////////////////////////
// private
////////////////////////////////////////////////////////////////////////////////

bool Collision::check_contact(){  
  // approximate (big distance -> no collision)
  float max_distance = phy_obj_0->get_size() + phy_obj_1->get_size();
  float distance = glm::distance(phy_obj_0->get_position(), phy_obj_0->get_position());
  
  if(distance > max_distance)
    return false;
    
  // check in more detail
  return check_contact_detailed();
}



//------------------------------------------------------------------------------
bool Collision::check_contact_detailed(){
  auto points_0 = fetch_points_world_space(phy_obj_0);
  auto points_1 = fetch_points_world_space(phy_obj_1);
  auto edges = fetch_edges(points_0, points_1);
  
  bool overlap;
  for(auto &e : edges){
    glm::vec2 edge_vector = e.p_0 - e.p_1;
    glm::vec2 axis = perpendicular(edge_vector);
    overlap = check_proj_overlap(
      project_polygon(axis, points_0),
      project_polygon(axis, points_1)
    );
    
    if( ! overlap)   // found separating line
      return false;
  }
  
  return true;   // no separating line found
}



//------------------------------------------------------------------------------
Collision::projection Collision::project_polygon(glm::vec2 axis, std::vector< glm::vec2 > polygon){
  if(polygon.size() < 1)
    throw std::runtime_error("Collision detection: Cannot check polygon with zero points.");
  
  float min = std::numeric_limits<float>::max();
  float max = std::numeric_limits<float>::min();
  
  // project point onto axis and compare to prev min/max
  for(auto &p : polygon){
    float proj = glm::dot(axis, p);
    
    if(proj < min)
      min = proj;
      
    if(proj > max)
      max = proj;
  }
  
  return {min, max};
}



//------------------------------------------------------------------------------
bool Collision::check_proj_overlap(Collision::projection proj_0, Collision::projection proj_1){
  if(proj_0.min < proj_1.min)
    return proj_1.min < proj_0.max;   // min0 ----- min1 -- max0 ----- max1
    
  return proj_0.min < proj_1.max;   // min1 ----- min0 -- max1 ----- max0
}



//------------------------------------------------------------------------------
glm::vec2 Collision::perpendicular(glm::vec2 vec){
  return glm::normalize( glm::vec2( - vec.y, vec.x) );
}



//------------------------------------------------------------------------------
std::vector< glm::vec2 > Collision::fetch_points_world_space(std::shared_ptr< PhyObject > phy_obj){
  auto points = phy_obj->get_points();
  glm::vec2 offset = phy_obj->get_position();
  float rotation = phy_obj->get_rotation();
  
  to_world_space(points, offset, rotation);
  return points;
}



//------------------------------------------------------------------------------
std::vector< Collision::edge > Collision::fetch_edges(std::vector< glm::vec2 > points_0, std::vector< glm::vec2 > points_1){
  std::vector< edge > ret;
  
  // polygon 0
  glm::vec2 prev_point = points_0.back();
  
  for(auto &p : points_0){
    ret.push_back( {prev_point, p} );
    prev_point = p;
  }
  
  // polygon 1
  prev_point = points_1.back();
  
  for(auto &p : points_1){
    ret.push_back( {prev_point, p} );
    prev_point = p;
  }
  
  return ret;
}



//------------------------------------------------------------------------------
void Collision::to_world_space(std::vector< glm::vec2 >& points, glm::vec2 offset, float rotation){
  // adjust to rotation
  float sine = sin( glm::radians(rotation) );
  float cosine = cos( glm::radians(rotation) );
  
  for(auto &p : points){
    float x = p.x * cosine - p.y * sine;
    float y = p.x * sine + p.y * cosine;
    p.x = x;
    p.y = y;
  }
  
  // adjust to world position
  for(auto &p : points){
    p.x += offset.x;
    p.y += offset.y;
  }
}



//------------------------------------------------------------------------------
void Collision::to_object_space(std::vector< glm::vec2 >& points, glm::vec2 offset, float rotation){
  // adjust to relative position
  for(auto &p : points){
    p.x -= offset.x;
    p.y -= offset.y;
  }
  
  // undo rotation
  float sine = sin( glm::radians(rotation) );
  float cosine = cos( glm::radians(rotation) );
  
  for(auto &p : points){
    float x = p.x * cosine + p.y * sine;
    float y = -(p.x * sine) + p.y * cosine;
    p.x = x;
    p.y = y;
  }
}


