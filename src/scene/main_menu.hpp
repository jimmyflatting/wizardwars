// main_menu.hpp
#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "scene.hpp"

class MainMenu : public Scene
{
public:
  void update() override;
  void render() override;
  void handle_events() override;
};

#endif // MAIN_MENU_HPP
