#ifndef FIELD_HPP
#define FIELD_HPP

#include <vector>


class Field {
public:
  Field(int width, int height);
  int GetWidth();
  int GetHeight();
  void PlaceObject(std::pair<int, int> position, char symbol);
  std::vector<std::vector<char>> GetData();
};

#endif // FIELD_HPP