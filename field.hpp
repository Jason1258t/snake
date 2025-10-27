#include "game_object.hpp"
#include <vector>

class GameField
{
private:
	int width;
	int height;
	std::vector<std::vector<GameObject*>> grid;

public:
	GameField(int w, int h) : width(w), height(h)
	{
		grid.resize(height, std::vector<GameObject*>(width, nullptr));
	}

	int getWidth() const { return width; }
	int getHeight() const { return height; }

	bool isPositionValid(const Vector2D& pos) const
	{
		return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
	}

	bool isPositionEmpty(const Vector2D& pos) const
	{
		if (!isPositionValid(pos))
			return false;

		if (grid[pos.y][pos.x] == nullptr)
			return true;

		return grid[pos.y][pos.x]->getType() == GameObjectType::EMPTY;
	}

	GameObject* getObjectAt(const Vector2D& pos) const
	{
		if (!isPositionValid(pos))
			return nullptr;
		return grid[pos.y][pos.x];
	}

	bool placeObject(GameObject* obj)
	{
		if (!obj || !isPositionValid(obj->getPosition()))
		{
			return false;
		}
		Vector2D pos = obj->getPosition();
		grid[pos.y][pos.x] = obj;
		return true;
	}

	bool removeObject(const Vector2D& pos)
	{
		if (!isPositionValid(pos))
			return false;
		grid[pos.y][pos.x] = nullptr;
		return true;
	}

	bool clearField()
	{
		for (auto& row : grid)
		{
			for (auto& cell : row)
			{
				delete cell; 
				cell = nullptr; 
			}
		}
		return true;
	}

	const std::vector<std::vector<GameObject*>>& getGrid() const
	{
		return grid;
	}
};
