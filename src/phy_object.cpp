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

#include "phy_object.h"

#include <iostream>
#include <math.h>



////////////////////////////////////////////////////////////////////////////////
// Object public
////////////////////////////////////////////////////////////////////////////////

PhyObject::PhyObject(glm::vec2 position, float rotation, float size, glm::vec3 colour, uint time, id window_id){
  this->position = position;
  this->rotation = fmod(rotation, 360.0f);
  this->size = size;
  this->colour = colour;
  this->time = time;
  this->window_id = window_id;
}



//------------------------------------------------------------------------------
PhyObject::~PhyObject(){
  if( ! Window::got_closed(window_id))
    Window::remove_gobject(window_id, gobj_id);
}



//------------------------------------------------------------------------------
uint PhyObject::get_time(){  return time;  }



//------------------------------------------------------------------------------
bool PhyObject::is_active(){  return activated;  }



//------------------------------------------------------------------------------
void PhyObject::update(){
  update_rotation();
  update_position();
}



//------------------------------------------------------------------------------
void PhyObject::set_position(glm::vec2 pos){
  position = pos;
  Window::set_gobj_position(window_id, gobj_id, {pos.x, pos.y, 0.0f});
}



//------------------------------------------------------------------------------
void PhyObject::set_rotation(float rot){
  rotation = fmod(rot, 360.0f);
  Window::set_gobj_rotation(window_id, gobj_id, rotation);
}



//------------------------------------------------------------------------------
glm::vec2 PhyObject::get_position(){  return position;  }



//------------------------------------------------------------------------------
float PhyObject::get_rotation(){  return rotation;  }



//------------------------------------------------------------------------------
float PhyObject::get_size(){  return size;  }



//------------------------------------------------------------------------------
std::vector< glm::vec2 > PhyObject::get_points(){  return points;  }



//------------------------------------------------------------------------------
glm::vec2 PhyObject::get_velocity(){  return velocity;  }



//------------------------------------------------------------------------------
float PhyObject::get_angular_velocity(){  return angular_velocity;  }



//------------------------------------------------------------------------------
float PhyObject::get_bounciness(){  return bounciness;  }



//------------------------------------------------------------------------------
float PhyObject::get_mass(){  return mass;  }



//------------------------------------------------------------------------------
float PhyObject::get_inertia_tensor(){  return inertia_tensor;  }



//------------------------------------------------------------------------------
void PhyObject::apply_force(glm::vec2 force, glm::vec2 pos){
  torque = cross_2d(force, pos);
}



//------------------------------------------------------------------------------
void PhyObject::apply_impulse(float impulse, glm::vec2 rel_coll_point, glm::vec2 coll_normal){
  // linear velocity
  velocity += impulse * coll_normal * (1.0f / mass);
  
  // angular velocity
  angular_velocity += cross_2d(rel_coll_point, impulse * coll_normal)  * (1.0f / inertia_tensor);
}



////////////////////////////////////////////////////////////////////////////////
// Object private
////////////////////////////////////////////////////////////////////////////////

void PhyObject::init(){
  calc_inertia_tensor();
  calc_center_of_mass();
}



//------------------------------------------------------------------------------
void PhyObject::calc_inertia_tensor(){
  inertia_tensor = 0.0f;
  float p_mass = mass / points.size();   // assume even mass distribution in rigidbodies
  
  for(auto &p : points)
    inertia_tensor += p_mass * glm::dot(p, p);
    
  inertia_tensor /= adjustment_const;
}



//------------------------------------------------------------------------------
void PhyObject::calc_center_of_mass(){
  center_of_mass = {0.0f, 0.0f};
  float p_mass = mass / points.size();   // assume even mass distribution in rigidbodies
  
  for(auto &p : points)
    center_of_mass += p_mass * p;
    
  center_of_mass /= points.size();
}



//------------------------------------------------------------------------------
void PhyObject::update_rotation(){
  set_rotation(rotation + step_time * angular_velocity);   // update phy & graphics
  
  float change = torque / inertia_tensor;
  angular_velocity = angular_velocity + (step_time * change);
  torque = 0.0f;
}



//------------------------------------------------------------------------------
void PhyObject::update_position(){
  set_position(position + step_time * velocity);   // update phy & graphics
}



//------------------------------------------------------------------------------
float PhyObject::cross_2d(glm::vec2 v_0, glm::vec2 v_1){
  return v_0.x * v_1.y - v_0.y - v_1.x;
}



////////////////////////////////////////////////////////////////////////////////
// Triangle public
////////////////////////////////////////////////////////////////////////////////

PhyTriangle::PhyTriangle(glm::vec2 position, float rotation, float size, glm::vec3 colour, uint time, id window_id)
  : PhyObject(position, rotation, size, colour, time, window_id){

    calc_points();
    init();
}



//------------------------------------------------------------------------------
PhyTriangle::~PhyTriangle(){}



//------------------------------------------------------------------------------
void PhyTriangle::activate(){
  glm::vec3 pos = {position.x, position.y, 0.0f};
  gobj_id = Window::add_gobject(window_id, t_triangle, pos, rotation, size, colour);
  activated = true;
}



////////////////////////////////////////////////////////////////////////////////
// Triangle private
////////////////////////////////////////////////////////////////////////////////

void PhyTriangle::calc_points(){
  points.clear();
  
  float height = size * sqrt(3) / 2;
  float third = 1.0f / 3.0f;
  
  points.push_back({ size / 2   , - third * height   });
  points.push_back({ - size / 2 , - third * height   });
  points.push_back({ 0.0f       , 2 * third * height });
}



////////////////////////////////////////////////////////////////////////////////
// Rect public
////////////////////////////////////////////////////////////////////////////////

PhyRect::PhyRect(glm::vec2 position, float rotation, float size, glm::vec3 colour, uint time, id window_id)
  : PhyObject(position, rotation, size, colour, time, window_id){

    calc_points();
    init();
}



//------------------------------------------------------------------------------
PhyRect::~PhyRect(){}



//------------------------------------------------------------------------------
void PhyRect::activate(){
  glm::vec3 pos = {position.x, position.y, 0.0f};
  gobj_id = Window::add_gobject(window_id, t_rectangle, pos, rotation, size, colour);
  activated = true;
}



////////////////////////////////////////////////////////////////////////////////
// Rect private
////////////////////////////////////////////////////////////////////////////////

void PhyRect::calc_points(){
  points.clear();
  
  float half = size / 2;
  
  points.push_back({ - half , - half });
  points.push_back({ - half , half   });
  points.push_back({ half   , half   });
  points.push_back({ half   , - half });
}



////////////////////////////////////////////////////////////////////////////////
// Circle public
////////////////////////////////////////////////////////////////////////////////

PhyCircle::PhyCircle(glm::vec2 position, float rotation, float size, glm::vec3 colour, uint time, id window_id)
  : PhyObject(position, rotation, size, colour, time, window_id){

    calc_points();
    init();
}



//------------------------------------------------------------------------------
PhyCircle::~PhyCircle(){}



//------------------------------------------------------------------------------
void PhyCircle::activate(){
  glm::vec3 pos = {position.x, position.y, 0.0f};
  gobj_id = Window::add_gobject(window_id, t_circle, pos, rotation, size, colour);
  activated = true;
}



////////////////////////////////////////////////////////////////////////////////
// Cirlce private
////////////////////////////////////////////////////////////////////////////////

void PhyCircle::calc_points(){
  points.clear();
  
  float half = size / 2;
  
  for(uint i = 0; i < point_count; i++){
    float segment = 360.0f * i / point_count;
    float y = half * sin( glm::radians(segment) );
    float x = half * cos( glm::radians(segment) );
    points.push_back( {x, y} );
  }
}


