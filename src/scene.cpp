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
using namespace std::chrono;



Scene::Scene(){
  window_id = Window::open("Uninitialised Name");
}



//------------------------------------------------------------------------------
Scene::~Scene(){}



//------------------------------------------------------------------------------
void Scene::set_name(const std::string& name){
  Window::set_window_name(window_id, name);
}



//------------------------------------------------------------------------------
void Scene::set_background_colour(glm::vec3 colour){
  Window::set_background_colour(window_id, colour);
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
    case triangle:  obj = std::make_shared<PhyTriangle>(pos, rot, size, colour, time, window_id); break;
    case rectangle: obj = std::make_shared<PhyRect>(pos, rot, size, colour, time, window_id); break;
    case circle:    obj = std::make_shared<PhyCircle>(pos, rot, size, colour, time, window_id); break;
    default: throw std::runtime_error("Invalid Phy_Object type");
  }
  
  this->phy_objects_wait.push(obj);
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
  
  // finished
  std::cout << "Done.\n";
  
  // wait for window to close
  while(true){
    if( Window::got_closed(window_id) )
      break;
    
    std::this_thread::sleep_for(10ms);
  }
}



//------------------------------------------------------------------------------
void Scene::loop_timer(){
  // init
  uint time_prev = current_time();
  uint time_diff = 0;
  
  // wait for tick
  while(ticks_passed < time && ! Window::got_closed(window_id)){
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
  static bool force_applied = false;
  if(phy_objects.size() > 1 && ! force_applied){
    phy_objects[0]->apply_force({10000.0f, 0.0f}, {1.0f, 1.0f});
    force_applied = true;
  }
}



//------------------------------------------------------------------------------
void Scene::check_activate_objects(){
  for( ; ! phy_objects_wait.empty(); phy_objects_wait.pop()){
    auto obj = phy_objects_wait.top();
    
    // activate
    if( obj->get_time() <= ticks_passed ){
      obj->activate();
      phy_objects.push_back(obj);
    }
    
    // skip and wait
    else
      break;
  }
}



//------------------------------------------------------------------------------
void Scene::update_objects(){
  // collisions
  handle_collisions();
  if(collisions.size() > 1000)
    collisions.clear();
  
  // update individual objects
  for(auto &o : phy_objects)
    o->update();
}



//------------------------------------------------------------------------------
void Scene::handle_collisions(){
  for(std::size_t i = 0; i < phy_objects.size(); i++){
    for(std::size_t j = i + 1; j < phy_objects.size(); j++){
      std::shared_ptr<Collision> col = std::make_shared<Collision>(phy_objects[i], phy_objects[j], window_id);
      col->handle();
      collisions.push_back(col);
    }
  }
}