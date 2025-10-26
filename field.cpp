#include "field.hpp"
#include <vector>

class Field {
public:
  Field(int width, int height) : width(width), height(height) {}
  int GetWidth() { return width; };
  int GetHeight() { return height; };

  void PlaceObject(std::pair<int, int> position, char symbol) {
    data[position.first][position.second] = symbol;
  }

  std::vector<std::vector<char>> GetData() { return data; };

private:
  int width;
  int height;
  std::vector<std::vector<char>> data;
};