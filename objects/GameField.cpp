#include "GameField.hpp"
#include <algorithm>

GameField::GameField(int w, int h) : width(w), height(h)
{
	InitializeGrid();
}

bool GameField::IsPositionValid(const Vector2D& pos) const
{
	return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
}

bool GameField::IsPositionEmpty(const Vector2D& pos) const
{
	if (!IsPositionValid(pos))
		return false;

	auto& cell = grid[pos.y][pos.x];
	if (!cell)
		return true;

	return cell->GetType() == GameObjectType::EMPTY;
}

Vector2D GameField::GetRandomEmptyPosition() const
{
	std::vector<Vector2D> emptyPositions = GetAllEmptyPositions();

	if (emptyPositions.empty())
	{
		return Vector2D{ -1, -1 };
	}

	std::uniform_int_distribution<> dis(0, emptyPositions.size() - 1);
	return emptyPositions[dis(gen)];
}

std::vector<Vector2D> GameField::GetAllEmptyPositions() const
{
	std::vector<Vector2D> emptyPositions;

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			Vector2D pos{ x, y };
			if (IsPositionEmpty(pos))
			{
				emptyPositions.push_back(pos);
			}
		}
	}

	return emptyPositions;
}

GameObject* GameField::GetObjectAt(const Vector2D& pos) const
{
	if (!IsPositionValid(pos))
		return nullptr;
	return grid[pos.y][pos.x].get();
}

bool GameField::PlaceObject(std::unique_ptr<GameObject> obj)
{
	if (!obj || !IsPositionValid(obj->GetPosition()))
		return false;

	Vector2D pos = obj->GetPosition();
	grid[pos.y][pos.x] = std::move(obj);
	return true;
}

bool GameField::RemoveObjectAt(const Vector2D& pos)
{
	if (!IsPositionValid(pos))
		return false;

	grid[pos.y][pos.x] = СreateEmptyObject(pos);
	return true;
}

void GameField::ClearField()
{
	InitializeGrid();
}

void GameField::InitializeGrid()
{
	grid.clear();
	grid.resize(height);
	for (int y = 0; y < height; ++y)
	{
		grid[y].resize(width);
		for (int x = 0; x < width; ++x)
		{
			grid[y][x] = СreateEmptyObject(Vector2D{ x, y });
		}
	}
}

std::unique_ptr<GameObject> GameField::СreateEmptyObject(const Vector2D& pos) const
{
	return std::make_unique<GameObject>(pos, GameObjectType::EMPTY);
}