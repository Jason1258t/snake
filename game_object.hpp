#pragma once

#include "config.hpp"
#include "vector_2d.hpp"

enum class GameObjectType
{
	EMPTY,
	SNAKE_HEAD,
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
private:
	bool isHead;

public:
	SnakeSegment(const Vector2D& pos, bool head = false) :
		GameObject(pos, head ? GameObjectType::SNAKE_HEAD : GameObjectType::SNAKE_BODY),
		isHead(head) {}

	bool setIsHead(bool head)
	{
		isHead = head;
		type = head ? GameObjectType::SNAKE_HEAD : GameObjectType::SNAKE_BODY;
		return true;
	}

	bool getIsHead() const { return isHead; }
};

class Apple : public GameObject
{
public:
	explicit Apple(const Vector2D& pos) : GameObject(pos, GameObjectType::APPLE) {}
};