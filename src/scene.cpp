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

PhyObject::PhyObject(glm::vec2 position, float rotation, float size, glm::vec3 colour, uint time){
  this->position = position;
  this->rotation = rotation;
  this->size = size;
  this->colour = colour;
  this->time = time;
}



//------------------------------------------------------------------------------
PhyObject::~PhyObject(){}



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
void Scene::add_object(PhyObject object){  this->phy_objects.push_back(object);  }



//------------------------------------------------------------------------------
void Scene::start(){
  if(name_ready && background_ready && time_ready)
    run();
    
  else
    throw std::runtime_error("Scene has not been initialized properly.");
}



////////////////////////////////////////////////////////////////////////////////
// Scene private
////////////////////////////////////////////////////////////////////////////////

void Scene::run(){
  window = std::make_shared<Window>(name);
}