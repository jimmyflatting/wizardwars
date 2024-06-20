#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

#include <vector>
#include "scene.hpp"

class SceneManager
{
public:
  void add_scene(Scene *scene);
  void change_scene(Scene *scene);
  void update();
  void render();
  void handle_events();

private:
  std::vector<Scene *> scenes;
  Scene *current_scene;
};

#endif // SCENE_MANAGER_HPP
