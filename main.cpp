#include "render.hpp"
#include "snake.hpp"
#include <memory>
#include <thread>
#include <chrono>
#include <functional>
#include <curses.h>

#define FRAME_TIME 250

void ReplaceApple(const Snake &snake, const Point &boundries, Apple &apple) {
  do {
    apple.Randomize(boundries);
  } while (snake.OnSnake(apple.GetPos()));
}

void ListenForKeys(const std::function<void(const Direction&&)> &turn, const std::shared_ptr<bool> playing) {
  while (*playing) {
    switch (getch())
    {
    case KEY_UP:
      turn(Direction::UP);
      break;
    case KEY_RIGHT:
      turn(Direction::RIGHT);
      break;
    case KEY_DOWN:
      turn(Direction::DOWN);
      break;
    case KEY_LEFT:
      turn(Direction::LEFT);
      break;
    default:
      break;
    }
  }
}

class Window {
private:
  WINDOW *m_window;
public:
  Window() {
    m_window = initscr();
    keypad(m_window, true);
    clear();
    refresh();
  }
  ~Window() {
    endwin();
  }
};

bool ColidesWithBoundries(const Point &boundries, const Snake &snake) {
  const auto &headPos = snake.Head();
  return (
    (headPos.x == 0 || headPos.x == boundries.x) || 
    (headPos.y == 0 || headPos.y == boundries.y)
  );
}

int main() {
  srand(time(0));
  Window();

  auto snake = std::make_shared<Snake>(Point(3, 3), Direction::DOWN);
  auto apple = std::make_unique<Apple>();
  const Point &bounderies = Point(20,15);
  ReplaceApple(*snake, bounderies, *apple);
  auto playing = std::make_shared<bool>(true);

  auto setDirection = ([snake](const Direction &&direction) -> void{snake->Turn(std::move(direction));});
  auto thread = std::thread(ListenForKeys, setDirection, playing);

  auto t0 = std::chrono::system_clock::now() - std::chrono::milliseconds(FRAME_TIME);
  while (*playing) {
    Render(bounderies, *snake, *apple);

    auto dt = std::chrono::system_clock::now() - t0;
    printw("Render time (nanos): %ld\n", static_cast<long>(dt.count()));
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_TIME) - dt);
    t0 = std::chrono::system_clock::now();

    snake->Grow();
    if (snake->Head() == apple->GetPos()) {
      ReplaceApple(*snake, bounderies, *apple);
    } else {
      snake->Retract();
    }

    if (ColidesWithBoundries(bounderies, *snake)) {
      *playing = false;
    }
  }

  GameOver();
  thread.join();
}
