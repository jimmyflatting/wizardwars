#ifndef SCENE_HPP
#define SCENE_HPP

class Scene
{
public:
  virtual ~Scene() {}
  virtual void update() = 0;
  virtual void render() = 0;
  virtual void handle_events() = 0;
};

#endif // SCENE_HPP
