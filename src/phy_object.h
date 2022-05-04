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

#include <vector>
#include <string>
#include <memory>

#include <glm/glm.hpp>   // sudo apt install libglm-dev

#include "../simple_2d_graphics/src/window.h"
#include "../simple_2d_graphics/src/graphics_object.h"



class PhyObject{
public:
  PhyObject(
    glm::vec2 position,
    float rotation,
    float size,
    glm::vec3 colour,
    uint time,
    id window_id
  );
  ~PhyObject();
  uint get_time();
  bool is_active();
  virtual void activate() = 0;
  void update();
  void set_position(glm::vec2 pos);
  void set_rotation(float rot);
  glm::vec2 get_position();
  float get_rotation();
  float get_size();
  std::vector< glm::vec2 > get_points();   // compiler should optimise 'return by value' for std types
  glm::vec2 get_velocity();
  float get_angular_velocity();
  float get_bounciness();
  float get_mass();
  void apply_force(glm::vec2 force, glm::vec2 position);
  
protected:
  id window_id;
  id gobj_id;
  uint time;
  bool activated = false;
  
  glm::vec2 position;
  float rotation;
  float size;
  glm::vec3 colour;
  std::vector< glm::vec2 > points;   // order of ponits matter!
  glm::vec2 center_of_mass = {0.0f, 0.0f};
  float intertia_tensor = 0.0f;
  float angular_velocity = 0.0f;
  float torque = 0.0f;
  float step_time = 1.0f / 100.0f;   // duration of tick in seconds
  float mass = 0.1f;
  float adjustment_const = 100.0f;   // adjust this until simulation looks good
  float bounciness = 0.5f;   // keep between 0 and 1 !
  glm::vec2 velocity = {0.0f, 0.0f};
  
  virtual void calc_points() = 0;
  void init();
  void calc_center_of_mass();
  void calc_inertia_tensor();
  void update_rotation();
  void update_position();
};



//------------------------------------------------------------------------------
class PhyTriangle : public PhyObject{
public:
  PhyTriangle(
    glm::vec2 position,
    float rotation,
    float size,
    glm::vec3 colour,
    uint time,
    id window_id
  );
  ~PhyTriangle();
  void activate();
  
protected:
  void calc_points();
};



//------------------------------------------------------------------------------
class PhyRect : public PhyObject{
public:
  PhyRect(
    glm::vec2 position,
    float rotation,
    float size,
    glm::vec3 colour,
    uint time,
    id window_id
  );
  ~PhyRect();
  void activate();
  
protected:
  void calc_points();
};



//------------------------------------------------------------------------------
class PhyCircle : public PhyObject{
public:
  PhyCircle(
    glm::vec2 position,
    float rotation,
    float size,
    glm::vec3 colour,
    uint time,
    id window_id
  );
  ~PhyCircle();
  void activate();
  
protected:
  uint point_count = 16;
  
  void calc_points();
};