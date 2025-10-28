#pragma once

#include "game_object.hpp"
#include <algorithm>
#include <memory>
#include <random>
#include <vector>

using ObjectsGrid = std::vector<std::vector<std::unique_ptr<GameObject>>>;

class GameField
{
private:
	int width;
	int height;
	ObjectsGrid grid;
	mutable std::mt19937 gen;

public:
	GameField(int w, int h) : width(w), height(h)
	{
		initializeGrid();
	}

	// --- Убираем возможность копирования ---
	GameField(const GameField&) = delete;
	GameField& operator=(const GameField&) = delete;

	// --- Разрешаем перемещение ---
	GameField(GameField&&) noexcept = default;
	GameField& operator=(GameField&&) noexcept = default;

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

		auto& cell = grid[static_cast<int>(pos.y)][static_cast<int>(pos.x)];
		if (!cell)
			return true;

		return cell->getType() == GameObjectType::EMPTY;
	}

	Vector2D getRandomEmptyPosition() const
	{
		std::vector<Vector2D> emptyPositions = getAllEmptyPositions();

		if (emptyPositions.empty())
		{
			return Vector2D{ -1, -1 };
		}

		std::uniform_int_distribution<> dis(0, emptyPositions.size() - 1);
		return emptyPositions[dis(gen)];
	}

	std::vector<Vector2D> getAllEmptyPositions() const
	{
		std::vector<Vector2D> emptyPositions;

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				Vector2D pos{ x, y };
				if (isPositionEmpty(pos))
				{
					emptyPositions.push_back(pos);
				}
			}
		}

		return emptyPositions;
	}

	GameObject* getObjectAt(const Vector2D& pos) const
	{
		if (!isPositionValid(pos))
			return nullptr;
		return grid[static_cast<int>(pos.y)][static_cast<int>(pos.x)].get();
	}

	bool placeObject(std::unique_ptr<GameObject> obj)
	{
		if (!obj || !isPositionValid(obj->getPosition()))
			return false;

		Vector2D pos = obj->getPosition();
		grid[static_cast<int>(pos.y)][static_cast<int>(pos.x)] = std::move(obj);
		return true;
	}

	bool removeObjectAt(const Vector2D& pos)
	{
		if (!isPositionValid(pos))
			return false;

		grid[static_cast<int>(pos.y)][static_cast<int>(pos.x)] = createEmptyObject(pos);
		return true;
	}

	void clearField()
	{
		initializeGrid();
	}

	const ObjectsGrid& getGrid() const
	{
		return grid;
	}

private:
	void initializeGrid()
	{
		grid.clear();
		grid.resize(height);
		for (int y = 0; y < height; ++y)
		{
			grid[y].resize(width);
			for (int x = 0; x < width; ++x)
			{
				grid[y][x] = createEmptyObject(Vector2D{ x, y });
			}
		}
	}
	std::unique_ptr<GameObject> createEmptyObject(const Vector2D& pos) const
	{
		return std::make_unique<GameObject>(pos, GameObjectType::EMPTY);
	}
};