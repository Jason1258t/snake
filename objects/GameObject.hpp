#pragma once

#include "../utils/Vector2D.hpp"

enum class GameObjectType
{
	EMPTY,
	SNAKE_BODY,
	APPLE
};

class GameObject
{
protected:
	Vector2D position;
	GameObjectType type;

public:
	GameObject(const Vector2D& pos, GameObjectType objType) :
		position(pos),
		type(objType) {}

	virtual ~GameObject() = default;

	Vector2D GetPosition() const { return position; }
	GameObjectType GetType() const { return type; }

	bool SetPosition(const Vector2D& newPos)
	{
		position = newPos;
		return true;
	}
};

class SnakeSegment : public GameObject
{
public:
	SnakeSegment(const Vector2D& pos) :
		GameObject(pos, GameObjectType::SNAKE_BODY) {}
};

class Apple : public GameObject
{
public:
	explicit Apple(const Vector2D& pos) : GameObject(pos, GameObjectType::APPLE) {}
};