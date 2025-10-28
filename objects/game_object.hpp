#pragma once

#include "../utils/vector_2d.hpp"

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

	Vector2D getPosition() const { return position; }
	GameObjectType getType() const { return type; }

	bool setPosition(const Vector2D& newPos)
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