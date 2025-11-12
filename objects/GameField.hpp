#pragma once

#include "GameObject.hpp"
#include <memory>
#include <vector>
#include <random>

using ObjectsGrid = std::vector<std::vector<std::unique_ptr<GameObject>>>;

class GameField
{
private:
    int width;
    int height;
    ObjectsGrid grid;
    mutable std::mt19937 gen;

public:
    GameField(int w, int h);

    // --- Убираем возможность копирования ---
    GameField(const GameField&) = delete;
    GameField& operator=(const GameField&) = delete;

    // --- Разрешаем перемещение ---
    GameField(GameField&&) noexcept = default;
    GameField& operator=(GameField&&) noexcept = default;

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    const ObjectsGrid& GetGrid() const { return grid; }

    bool IsPositionValid(const Vector2D& pos) const;
    bool IsPositionEmpty(const Vector2D& pos) const;
    Vector2D GetRandomEmptyPosition() const;
    std::vector<Vector2D> GetAllEmptyPositions() const;
    GameObject* GetObjectAt(const Vector2D& pos) const;
    bool PlaceObject(std::unique_ptr<GameObject> obj);
    bool RemoveObjectAt(const Vector2D& pos);
    void ClearField();

private:
    void InitializeGrid();
    std::unique_ptr<GameObject> СreateEmptyObject(const Vector2D& pos) const;
};