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



////////////////////////////////////////////////////////////////////////////////
// Object public
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



//------------------------------------------------------------------------------
uint PhyObject::get_time(){  return time;  }



//------------------------------------------------------------------------------
bool PhyObject::is_active(){  return activated;  }



////////////////////////////////////////////////////////////////////////////////
// Object private
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Triangle public
////////////////////////////////////////////////////////////////////////////////

PhyTriangle::PhyTriangle(glm::vec2 position, float rotation, float size, glm::vec3 colour, uint time, std::shared_ptr<Window> window)
  : PhyObject(position, rotation, size, colour, time){
    
    this->window = window;
}



//------------------------------------------------------------------------------
PhyTriangle::~PhyTriangle(){}



//------------------------------------------------------------------------------
void PhyTriangle::activate(){
  glm::vec3 pos = {position.x, position.y, 0.0f};
  gobj_id = window->add_gobject(
    std::make_shared<GTriangle>(pos, rotation, size, colour)
  );
  
  activated = true;
}



////////////////////////////////////////////////////////////////////////////////
// Triangle private
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Rect public
////////////////////////////////////////////////////////////////////////////////

PhyRect::PhyRect(glm::vec2 position, float rotation, float size, glm::vec3 colour, uint time, std::shared_ptr<Window> window)
  : PhyObject(position, rotation, size, colour, time){
    
    this->window = window;
}



//------------------------------------------------------------------------------
PhyRect::~PhyRect(){}



//------------------------------------------------------------------------------
void PhyRect::activate(){
  glm::vec3 pos = {position.x, position.y, 0.0f};
  gobj_id = window->add_gobject(
    std::make_shared<GRect>(pos, rotation, size, colour)
  );
  
  activated = true;
}



////////////////////////////////////////////////////////////////////////////////
// Rect private
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Circle public
////////////////////////////////////////////////////////////////////////////////

PhyCircle::PhyCircle(glm::vec2 position, float rotation, float size, glm::vec3 colour, uint time, std::shared_ptr<Window> window)
  : PhyObject(position, rotation, size, colour, time){
    
    this->window = window;
}



//------------------------------------------------------------------------------
PhyCircle::~PhyCircle(){}



//------------------------------------------------------------------------------
void PhyCircle::activate(){
  glm::vec3 pos = {position.x, position.y, 0.0f};
  gobj_id = window->add_gobject(
    std::make_shared<GCircle>(pos, rotation, size, colour)
  );
  
  activated = true;
}



////////////////////////////////////////////////////////////////////////////////
// Cirlce private
////////////////////////////////////////////////////////////////////////////////