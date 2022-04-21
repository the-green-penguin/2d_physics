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

#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <unordered_map>
#include <queue>

///#define GLFW_INCLUDE_NONE     // not needed/working on Ubuntu, etc.
// #include <glad/gl.h>     // not needed/working on Ubuntu, etc.
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>   // sudo apt install libglm-dev
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_program.h"
#include "graphics_object.h"
#include "camera.h"



typedef unsigned long long int id;   // if you call "add_gobject()" more than 2^64 times, it's your problem!

typedef struct{
  std::unordered_map< id, std::shared_ptr< GShape > > data;
  std::mutex lock;
} sync_gobjects;

typedef struct{
  Camera data;
  std::mutex lock;
} sync_camera;

typedef struct{
  std::queue<id> data;
  std::mutex lock;
} sync_ids_to_delete;

typedef struct{
  glm::vec3 data;
  std::mutex lock;
} sync_background_colour;

typedef struct{
  std::string data;
  std::mutex lock;
} sync_name;



// .cpp contains non-member function!
class Window{
private:
////////////////////////////////////////////////////////////////////////////////
  class Window_Helper{
  public:
    // these are accessed by another thread -> lock needed
    sync_gobjects graphics_objects;
    sync_camera camera;
    sync_ids_to_delete ids_to_delete;
    std::atomic< bool > setup_ready = false;
    std::atomic< bool > closed = false;
    std::atomic< bool > allow_zoom = false;
    std::atomic< bool > allow_camera_movement = false;
    std::atomic< bool > clear_gobjects = false;
    sync_background_colour background_colour;
    sync_name window_name;
    
    Window_Helper(
      const std::string& window_name,
      Window* parent
    );
    ~Window_Helper();
    void run();
    
  private:
    Window* parent;
    GLFWwindow* window;
    int width, height;
    std::shared_ptr< Shader_Program > shader_program;
    
    void init();
      void setup_glfw();
      void setup_glew();
      void setup_glfw_debugging();
      void setup_shader_program();
    void stop();
    void update_name();
    void render();
      void set_background();
      void delete_old_gobjects();
      void render_gobjects();
  };
////////////////////////////////////////////////////////////////////////////////
  
  std::shared_ptr< Window_Helper > helper;
  std::thread helper_thread;
  id next_id = 0;
  
  
  
public:  
  Window(const std::string& window_name);
  ~Window();
  void wait_for_setup();
  id add_gobject(std::shared_ptr< GShape > gobject);
  void remove_gobject(id id);
  void clear_gobjects();
  void set_gobj_position(id id, glm::vec3 pos);
  void set_gobj_rotation(id id, float rot);
  void set_camera_position(glm::vec3 pos);
  void set_camera_zoom(float zoom);
  void mod_camera_zoom(float zoom_diff);
  bool got_closed();
  void set_allow_zoom(bool b);
  void set_allow_camera_movement(bool b);
  void set_background_colour(glm::vec3 colour);
  void set_window_name(const std::string& name);
};