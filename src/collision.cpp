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
  
  approximate_coll_point();
  std::cout << coll_point.x << " " << coll_point.y << "\n";
}



////////////////////////////////////////////////////////////////////////////////
// private
////////////////////////////////////////////////////////////////////////////////

void Collision::get_points(){
  points_0 = phy_obj_0->get_points();
  points_1 = phy_obj_1->get_points();
  center_0 = phy_obj_0->get_position();
  center_1 = phy_obj_1->get_position();
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
  get_points();
  get_edges();
  
  // approximate (big distance -> no collision)
  float max_distance = phy_obj_0->get_size() + phy_obj_1->get_size();
  float distance = glm::distance(center_0, center_1);
  
  if(distance > max_distance)
    return false;
    
  // check in more detail
  return check_contact_detailed();
}



//------------------------------------------------------------------------------
bool Collision::check_contact_detailed(){
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
void Collision::get_surface_points(){
  // polygon 0
  glm::vec2 prev_point = points_0.back();
  for(auto &p : points_0){
    surface_points_0.push_back( p );
    surface_points_0.push_back( (prev_point + p) * 0.5f );
    prev_point = p;
  }
  
  // polygon 1
  prev_point = points_1.back();
  for(auto &p : points_1){
    surface_points_1.push_back( p );
    surface_points_1.push_back( (prev_point + p) * 0.5f );
    prev_point = p;
  }
}



//------------------------------------------------------------------------------
void Collision::approximate_coll_point(){
  get_surface_points();
  float distance = glm::distance(center_0, center_1);
  
  // approximate collision point inside polygon 0
  glm::vec2 approx_p0 = {0.0f, 0.0f};
  for(auto &p : surface_points_0){
    float impact_contribution = (glm::distance(p, center_1) - distance) / distance;
    if(impact_contribution > 0)
      approx_p0 += impact_contribution * p;
  }
  approx_p0 *= 1.0f / surface_points_0.size();
  
  // approximate collision point inside polygon 1
  glm::vec2 approx_p1 = {0.0f, 0.0f};
  for(auto &p : surface_points_1){
    float impact_contribution = (glm::distance(p, center_0) - distance) / distance;
    if(impact_contribution > 0)
      approx_p1 += impact_contribution * p;
  }
  approx_p1 *= 1.0f / surface_points_1.size();
  
  // result
  coll_point = (approx_p0 + approx_p1) * 0.5f;
}