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

#include "scene.h"

#include <iostream>
#include <exception>
#include <chrono>



////////////////////////////////////////////////////////////////////////////////
// PhyObject public
////////////////////////////////////////////////////////////////////////////////

PhyObject::PhyObject(glm::vec2 position, float rotation, float size, glm::vec3 colour, uint time, phy_obj_type type){
  this->position = position;
  this->rotation = rotation;
  this->size = size;
  this->colour = colour;
  this->time = time;
  this->type = type;
}



//------------------------------------------------------------------------------
PhyObject::~PhyObject(){}



//------------------------------------------------------------------------------
void PhyObject::set_id(id obj_id){  this->obj_id = obj_id;  }



//------------------------------------------------------------------------------
id PhyObject::get_id(){  return obj_id;  }



//------------------------------------------------------------------------------
uint PhyObject::get_time(){  return time;  }



//------------------------------------------------------------------------------
bool PhyObject::is_active(){  return activated;  }



//------------------------------------------------------------------------------
void PhyObject::activate(){  activated = true;  }



////////////////////////////////////////////////////////////////////////////////
// PhyObject private
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// Scene public
////////////////////////////////////////////////////////////////////////////////

Scene::Scene(){}



//------------------------------------------------------------------------------
Scene::~Scene(){}



//------------------------------------------------------------------------------
void Scene::set_name(const std::string& name){
  this->name = name;
  name_ready = true;
}



//------------------------------------------------------------------------------
void Scene::set_background_colour(glm::vec3 colour){
  this->background_colour = colour;
  background_ready = true;
}



//------------------------------------------------------------------------------
void Scene::set_time(uint time){
  this->time = time;
  time_ready = true;
}



//------------------------------------------------------------------------------
void Scene::add_object(std::shared_ptr<PhyObject> obj){
  this->phy_objects_wait.push_back(obj);
}



//------------------------------------------------------------------------------
void Scene::start(){
  if( is_ready() )
    run();
    
  else
    throw std::runtime_error("Scene has not been initialized properly.");
}



////////////////////////////////////////////////////////////////////////////////
// Scene private
////////////////////////////////////////////////////////////////////////////////

bool Scene::is_ready(){
  return name_ready && background_ready && time_ready;
}



//------------------------------------------------------------------------------
void Scene::run(){
  window = std::make_unique<Window>(name);
  window->wait_for_setup();
  loop_timer();
  std::cout << "Done.\n";
}



//------------------------------------------------------------------------------
void Scene::loop_timer(){
  // init
  using namespace std::chrono;
  uint time_prev = current_time();
  uint time_diff = 0;
  
  // wait for tick
  while(ticks_passed < time && ! window->got_closed()){
    // time since last check
    time_diff += current_time() - time_prev;
    
    // tick
    if(time_diff > 9900){
      loop_tick();
      time_diff = 0;
    }
    
    // next wait
    time_prev = current_time();
    std::this_thread::sleep_for(500us);
  }
}



//------------------------------------------------------------------------------
uint Scene::current_time(){
  using namespace std::chrono;
  
  auto now = time_point_cast<microseconds>(steady_clock::now());
  auto time = now.time_since_epoch();
  auto time_ms = std::chrono::duration_cast<std::chrono::microseconds>(time);
  
  return time_ms.count();
}



//------------------------------------------------------------------------------
void Scene::loop_tick(){
  check_activate_objects();
  ticks_passed++;
}



//------------------------------------------------------------------------------
void Scene::check_activate_objects(){
  for(auto &o : phy_objects_wait){
    if(o->get_time() <= ticks_passed && ! o->is_active())
      activate_object(o);
  }
}



//------------------------------------------------------------------------------
void Scene::activate_object(std::shared_ptr<PhyObject> obj){
  // safety
  if( ! is_ready() )
    throw std::runtime_error("Scene needs to be initialized properly before Phy_Objects can be added!");
  
  // add graphics_object
  glm::vec3 pos = {obj->position.x, obj->position.y, 0.0f};
  id id;
  
  switch(obj->type){
    case triangle:{
      id = window->add_gobject(
        std::make_shared<GTriangle>(pos, obj->rotation, obj->size, obj->colour)
      );
      break;
    }
    case rectangle:{
      id = window->add_gobject(
        std::make_shared<GRect>(pos, obj->rotation, obj->size, obj->colour)
      );
      break;
    }
    case circle:{
      id = window->add_gobject(
        std::make_shared<GCircle>(pos, obj->rotation, obj->size, obj->colour)
      );
      break;
    }
    default: throw std::runtime_error("Invalid Phy_Object type");
  }
  
  // store phy/g-objects
  obj->set_id(id);
  obj->activate();
  this->phy_objects.push_back(obj);
}