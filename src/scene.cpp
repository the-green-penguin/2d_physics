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
  
  ///glm::vec3 pos = {position.x, position.y, 0.0f};
  ///
  ///switch(type){
  ///  case triangle:  std::make_shared<GTriangle>(pos, rotation, size, colour);   break;
  ///  case rectangle: std::make_shared<GRect>(pos, rotation, size, colour);   break;
  ///  case circle:    std::make_shared<GCircle>(pos, rotation, size, colour);   break;
  ///  default: throw std::runtime_error("Invalid Phy_Object type");
  ///}
}



//------------------------------------------------------------------------------
PhyObject::~PhyObject(){}



//------------------------------------------------------------------------------
void PhyObject::set_gobj_id(id id){  this->g_obj_id = id;  }



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
  window = std::make_shared<Window>(name);
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
  // safety
  if( ! is_ready() )
    throw std::runtime_error("Scene needs to be initialized correctly before Phy_Objects can be added!");
  
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
  obj->set_gobj_id(id);
  this->phy_objects.push_back(obj);
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
  window = std::make_shared<Window>(name);
}