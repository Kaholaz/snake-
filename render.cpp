#include "render.hpp"
#include <cstring>
#include <curses.h>

Renderer::Renderer() {
  m_window = initscr();
  keypad(m_window, true);
  clear();
  refresh();
}

Renderer::~Renderer() { endwin(); }

void Renderer::Clear() const { clear(); }

void Renderer::Render(const Point &bounderies, const Snake &snake,
                      const Apple &apple) const {
  Clear();

  auto applePos = apple.GetPos();
  auto head = snake.Head();

  printw("Bounds:    %s\n", bounderies.ToString()->c_str());
  printw("Head:      %s\n", head.ToString()->c_str());
  printw("Apple:     %s\n", applePos.ToString()->c_str());
  printw("Direction: %s\n", ToString(snake.GetDirection())->c_str());

  for (size_t y = 0; y <= bounderies.y; ++y) {
    for (size_t x = 0; x <= bounderies.x; ++x) {
      if (x == 0 || y == 0 || x == bounderies.x || y == bounderies.y) {
        printw("X");
      } else if (x == applePos.x && y == applePos.y) {
        printw("o");
      } else if (snake.OnSnake(Point(x, y))) {
        printw("#");
      } else {
        printw(" ");
      }
    }
    printw("\n");
  }
  refresh();
}

void Renderer::GameOver() const {
  Clear();
  printw("---- GAME OVER! ----\n");
  refresh();
}
