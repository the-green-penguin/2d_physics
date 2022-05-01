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
#include <queue>
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
  uint time;
  std::vector< std::shared_ptr<PhyObject> > phy_objects;
  id window_id;
  uint ticks_passed = 0;
  
  // this mess is a priority_queue with phy_objects to be activated next on top
  static bool compare_time(std::shared_ptr< PhyObject > phy_0, std::shared_ptr< PhyObject > phy_1){
    return phy_0->get_time() > phy_1->get_time();   // object with biggest time value should end up on top
  }
  std::priority_queue<
    std::shared_ptr< PhyObject >,   // objec type
    std::vector< std::shared_ptr< PhyObject > >,   // container type
    decltype(&compare_time)   // comparison function
  > phy_objects_wait{compare_time};
  
  void run();
  void loop_timer();
    uint current_time();
    void loop_tick();
      void check_activate_objects();
        void activate_object(id obj_id);
        void remove_active_objects();
      void update_objects();
        void handle_collisions();
};