#include "render.hpp"
#include "snake.hpp"
#include <chrono>
#include <curses.h>
#include <functional>
#include <memory>
#include <thread>

#define FRAME_TIME 250

void ReplaceApple(const Snake &snake, const Point &boundries, Apple &apple) {
  do {
    apple.Randomize(boundries);
  } while (snake.OnSnake(apple.GetPos()));
}

void ListenForKeys(const std::function<void(const Direction &&)> &turn,
                   const std::shared_ptr<bool> playing) {
  while (*playing) {
    switch (getch()) {
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

int main() {
  srand(time(0));
  const auto render = Renderer();

  auto snake = std::make_shared<Snake>(Point(3, 3), Direction::DOWN);
  auto apple = std::make_unique<Apple>();
  const Point &bounderies = Point(20, 15);
  ReplaceApple(*snake, bounderies, *apple);
  auto playing = std::make_shared<bool>(true);

  auto setDirection = ([snake](const Direction &&direction) -> void {
    snake->Turn(std::move(direction));
  });
  auto thread = std::thread(ListenForKeys, setDirection, playing);

  auto t0 =
      std::chrono::system_clock::now() - std::chrono::milliseconds(FRAME_TIME);
  while (*playing) {
    render.Render(bounderies, *snake, *apple);

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

    if (snake->CollidesWithBoundries(bounderies)) {
      *playing = false;
    }
  }

  render.GameOver();
  thread.join();
}
