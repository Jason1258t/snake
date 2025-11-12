#pragma once

#include "../utils/Vector2D.hpp"
#include "GameObject.hpp"
#include <deque>
#include <memory>

class Snake
{
private:
	std::deque<std::unique_ptr<SnakeSegment>> segments;
	Vector2D currentDirection;

public:
	Snake(std::deque<std::unique_ptr<SnakeSegment>>&& initSegments,
		const Vector2D& initialDirection);

	// Правило пяти
	Snake(const Snake& other);
	Snake(Snake&& other) = default;
	Snake& operator=(const Snake& other);
	Snake& operator=(Snake&& other) = default;
	~Snake() = default;

	Snake withDirection(const Vector2D& newDirection) const;

	Snake move() const;
	Snake grow() const;

	Vector2D getHeadPosition() const { return segments.front()->GetPosition(); }
	const std::deque<std::unique_ptr<SnakeSegment>>& getSegments() const { return segments; }
	int getLength() const { return segments.size(); }
	Vector2D getCurrentDirection() const { return currentDirection; }

	bool checkSelfCollision() const;

private:
	Vector2D calculateNewHeadPosition() const;
	std::unique_ptr<SnakeSegment> createNewHead() const;
	std::deque<std::unique_ptr<SnakeSegment>> copySegments() const;
};