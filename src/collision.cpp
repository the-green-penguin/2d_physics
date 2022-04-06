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
  get_points();
  get_edges();
  contact = check_contact();
}



//------------------------------------------------------------------------------
Collision::~Collision(){}



//------------------------------------------------------------------------------
bool Collision::has_contact(){  return contact;  }



////////////////////////////////////////////////////////////////////////////////
// private
////////////////////////////////////////////////////////////////////////////////

void Collision::get_points(){
  points_0 = phy_obj_0->get_points();
  points_1 = phy_obj_1->get_points();
}



//------------------------------------------------------------------------------
void Collision::get_edges(){
  // polygon 0
  glm::vec2 prev_point = points_0.back();
  
  for(auto &p : points_0){
    edges.push_back( {prev_point, p} );
    prev_point = p;
  }
  
  // polygon 1
  prev_point = points_1.back();
  
  for(auto &p : points_1){
    edges.push_back( {prev_point, p} );
    prev_point = p;
  }
}



//------------------------------------------------------------------------------
bool Collision::check_contact(){
  // approximate (big distance -> no collision)
  float max_distance = phy_obj_0->get_size() + phy_obj_1->get_size();
  float distance = glm::distance(
    phy_obj_0->get_position(),
    phy_obj_1->get_position()
  );
  
  if(distance > max_distance)
    return false;
    
  // check in more detail
  return check_contact_detailed();
}



//------------------------------------------------------------------------------
bool Collision::check_contact_detailed(){
  bool overlap;
  
  for(auto &e : edges){
    glm::vec2 axis = perpendicular(e.point_0 - e.point_1);
    overlap = check_proj_overlap(
      project_polygon(axis, points_0),
      project_polygon(axis, points_1)
    );
    
    if( ! overlap)
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
  
  for(auto &p : polygon){
    float proj = glm::dot(axis, p);
    
    if(proj < min)
      min = proj;
      
    else if(proj > max)
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