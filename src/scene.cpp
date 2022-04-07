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

#include "collision.h"



Scene::Scene(){
  window = std::make_shared<Window>("Uninitialised Name");
  window->wait_for_setup();
}



//------------------------------------------------------------------------------
Scene::~Scene(){}



//------------------------------------------------------------------------------
void Scene::set_name(const std::string& name){
  window->set_window_name(name);
}



//------------------------------------------------------------------------------
void Scene::set_background_colour(glm::vec3 colour){
  window->set_background_colour(background_colour);
}



//------------------------------------------------------------------------------
void Scene::set_time(uint time){
  this->time = time;
}



//------------------------------------------------------------------------------
void Scene::add_object(glm::vec2 pos, float rot, float size, glm::vec3 colour, uint time, phy_obj_type type){
  // add objects
  std::shared_ptr<PhyObject> obj;
  switch(type){
    case triangle:  obj = std::make_shared<PhyTriangle>(pos, rot, size, colour, time, window); break;
    case rectangle: obj = std::make_shared<PhyRect>(pos, rot, size, colour, time, window); break;
    case circle:    obj = std::make_shared<PhyCircle>(pos, rot, size, colour, time, window); break;
    default: throw std::runtime_error("Invalid Phy_Object type");
  }
  
  this->phy_objects_wait.insert({next_id++, obj});
}



//------------------------------------------------------------------------------
void Scene::start(){
  run();  
}



////////////////////////////////////////////////////////////////////////////////
// private
////////////////////////////////////////////////////////////////////////////////

void Scene::run(){
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
  update_objects();
  ticks_passed++;
  
  // test
  if(phy_objects.size() > 1){
    phy_objects[0]->apply_force({100.0f, 0.0f}, {1.0f, 1.0f});
  }
}



//------------------------------------------------------------------------------
void Scene::check_activate_objects(){
  for(auto &o : phy_objects_wait){
    if(o.second->get_time() <= ticks_passed && ! o.second->is_active())
      activate_object(o.first);
  }
}



//------------------------------------------------------------------------------
void Scene::activate_object(id obj_id){  
  phy_objects_wait.at(obj_id)->activate();
  phy_objects.push_back( phy_objects_wait.at(obj_id) );
  phy_objects_wait.erase(obj_id);
}



//------------------------------------------------------------------------------
void Scene::update_objects(){
  // collisions
  handle_collisions();
  
  // update individual objects
  for(auto &o : phy_objects)
    o->update();
}



//------------------------------------------------------------------------------
void Scene::handle_collisions(){
  for(std::size_t i = 0; i < phy_objects.size(); i++){
    for(std::size_t j = i + 1; j < phy_objects.size(); j++){
      Collision col(phy_objects[i], phy_objects[j]);
      col.handle();
    }
  }
}