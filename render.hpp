#ifndef RENDER_HPP
#define RENDER_HPP
#include "snake.hpp"

class Renderer {
private:
  WINDOW *m_window;
  void Clear() const;

public:
  Renderer();
  ~Renderer();
  void Render(const Point &bounderies, const Snake &snake,
              const Apple &apple) const;
  void GameOver() const;
};

#endif