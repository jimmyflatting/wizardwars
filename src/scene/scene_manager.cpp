#include "scene_manager.hpp"

void SceneManager::add_scene(Scene *scene)
{
  scenes.push_back(scene);
}

void SceneManager::change_scene(Scene *scene)
{
  current_scene = scene;
}

void SceneManager::update()
{
  if (current_scene)
  {
    current_scene->update();
  }
}

void SceneManager::render()
{
  if (current_scene)
  {
    current_scene->render();
  }
}

void SceneManager::handle_events()
{
  if (current_scene)
  {
    current_scene->handle_events();
  }
}
