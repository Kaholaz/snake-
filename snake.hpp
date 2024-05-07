#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>

enum Direction {
  UP,
  RIGHT,
  DOWN,
  LEFT,
};
std::ostream &operator<<(std::ostream &ostream, const Direction &dir);
const std::unique_ptr<std::string> ToString(const Direction &dir);

class Point {
public:
  const int x;
  const int y;

  Point(const Point &p);
  Point(const int &x, const int &y);
  ~Point() = default;

  Point operator+(const Point &other) const;
  const std::unique_ptr<std::string> ToString() const;
  const bool operator==(const Point &other) const;
  static Point FromDirection(const Direction &direction);

  friend std::ostream &operator<<(std::ostream &ostream, const Point &point);
};

class Snake {
private:
  std::deque<std::unique_ptr<Point>> m_snake;
  Direction m_direction;
  Direction m_to_turn;
  std::mutex lock_direction;

public:
  Snake(const Point &startPos, const Direction &startDirection);
  ~Snake() = default;

  const Point &Head() const;
  void Turn(const Direction &&direction);
  const Direction &GetDirection() const;
  const bool OnSnake(const Point &point) const;
  const bool OnSnakeBody(const Point &point) const;
  const bool CollidesWithBoundries(const Point &boundries) const;

  void Grow();
  void Retract();
};

class Apple {
private:
  std::unique_ptr<Point> m_pos;

public:
  Apple();
  ~Apple() = default;
  void Randomize(const Point &boundries);
  const Point &GetPos() const;
};

#endif