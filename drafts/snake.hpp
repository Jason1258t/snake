#include "game_object.hpp"
#include <vector>

enum MoveDirection { left, top, right, bottom };

class Snake {
public:
  Snake(std::pair<int, int> position);
  std::vector<GameObject> getObjectsToRender();
  bool crossObject(GameObject obj);
  GameObject GetHead();
  MoveDirection GetDirection();
  void eatApple();
  bool headTouchPoint(std::pair<int, int> point);
  void changeDir();
  void move();
};