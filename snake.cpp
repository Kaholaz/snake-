#include "snake.hpp"
#include <cstring>
#include <curses.h>
#include <functional>
#include <memory>

std::ostream &operator<<(std::ostream &ostream, const Direction &dir) {
  switch (dir) {
  case Direction::UP:
    return ostream << "UP";
  case Direction::RIGHT:
    return ostream << "RIGHT";
  case Direction::DOWN:
    return ostream << "DOWN";
  case Direction::LEFT:
    return ostream << "LEFT";
  default:
    return ostream;
  }
}
const std::unique_ptr<std::string> ToString(const Direction &dir) {
  switch (dir) {
  case Direction::UP:
    return std::make_unique<std::string>("UP");
  case Direction::RIGHT:
    return std::make_unique<std::string>("RIGHT");
  case Direction::DOWN:
    return std::make_unique<std::string>("DOWN");
  case Direction::LEFT:
    return std::make_unique<std::string>("LEFT");
  default:
    return std::make_unique<std::string>();
  }
}

Point::Point(const Point &point) : x(point.x), y(point.y){};
Point::Point(const int &x, const int &y) : x(x), y(y){};
Point Point::operator+(const Point &other) const {
  return Point(x + other.x, y + other.y);
}
const std::unique_ptr<std::string> Point::ToString() const {
  return std::make_unique<std::string>("Point(=" + std::to_string(x) +
                                       ", y=" + std::to_string(y) + ")");
}
const bool Point::operator==(const Point &other) const {
  return x == other.x && y == other.y;
};
std::ostream &operator<<(std::ostream &ostream, const Point &point) {
  return ostream << "Point(x=" << point.x << ", y=" << point.y << ")";
}

Point Point::FromDirection(const Direction &direction) {
  switch (direction) {
  case Direction::UP:
    return Point(0, -1);
  case Direction::RIGHT:
    return Point(1, 0);
  case Direction::DOWN:
    return Point(0, 1);
  case Direction::LEFT:
    return Point(-1, 0);
  default:
    return Point(0, 0);
  }
}

Snake::Snake(const Point &startPos, const Direction &startDirection)
    : m_direction(startDirection), m_to_turn(startDirection), m_snake() {
  m_snake.emplace_back(std::make_unique<Point>(startPos));
}

void Snake::Turn(const Direction &&direction) {
  printw("%s\n", ToString(direction)->c_str());
  std::lock_guard lg(lock_direction);

  if (((m_direction == Direction::UP) && (direction == Direction::DOWN)) ||
      ((m_direction == Direction::LEFT) && (direction == Direction::RIGHT)) ||
      ((m_direction == Direction::DOWN) && (direction == Direction::UP)) ||
      ((m_direction == Direction::RIGHT) && (direction == Direction::LEFT))) {
    return;
  }

  m_to_turn = direction;
}

const Direction &Snake::GetDirection() const { return m_direction; }

const Point &Snake::Head() const { return *m_snake.back(); }

void Snake::Grow() {
  m_direction = m_to_turn;
  const Point newPos = Head() + Point::FromDirection(m_direction);
  m_snake.emplace_back(std::make_unique<Point>(newPos));
}

void Snake::Retract() { m_snake.pop_front(); }

const bool Snake::OnSnake(const Point &point) const {
  return (std::find_if(m_snake.begin(), m_snake.end(),
                       [&point](const std::unique_ptr<Point> &p) -> bool {
                         return point == *p;
                       }) != m_snake.end());
}

const bool Snake::OnSnakeBody(const Point &point) const {
  return (std::find_if(m_snake.begin(), m_snake.end() - 1,
                       [&point](const std::unique_ptr<Point> &p) -> bool {
                         return point == *p;
                       }) != m_snake.end());
}

const bool Snake::CollidesWithBoundries(const Point &boundries) const {
  const auto &headPos = Head();
  return ((headPos.x == 0 || headPos.x == boundries.x) ||
          (headPos.y == 0 || headPos.y == boundries.y));
}

Apple::Apple() { m_pos = std::make_unique<Point>(0, 0); }
void Apple::Randomize(const Point &boundries) {
  int x = std::rand() % (boundries.x - 1) + 1;
  int y = std::rand() % (boundries.y - 1) + 1;
  m_pos = std::make_unique<Point>(x, y);
}
const Point &Apple::GetPos() const { return *m_pos; }
