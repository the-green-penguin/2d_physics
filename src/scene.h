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
#include <unordered_map>
#include <string>
#include <memory>

#include <glm/glm.hpp>   // sudo apt install libglm-dev

#include "../simple_2d_graphics/src/window.h"
#include "phy_object.h"



enum phy_obj_type{
  triangle,
  rectangle,
  circle
};



class Scene{
public:
  Scene();
  ~Scene();
  void set_name(const std::string& name);
  void set_background_colour(glm::vec3 colour);
  void set_time(uint time);
  void add_object(
    glm::vec2 position,
    float rotation,
    float size,
    glm::vec3 colour,
    uint time,
    phy_obj_type type
  );
  void start();
  
private:
  std::string name;
  glm::vec3 background_colour;
  uint time;
  std::unordered_map<
    id, std::shared_ptr<PhyObject>
  > phy_objects_wait;
  id next_id = 0;
  std::vector< std::shared_ptr<PhyObject> > phy_objects;
  std::shared_ptr<Window> window;
  uint ticks_passed = 0;
  
  void run();
  void loop_timer();
    uint current_time();
    void loop_tick();
      void check_activate_objects();
        void activate_object(id obj_id);
      void update_objects();
        void handle_collisions();
};