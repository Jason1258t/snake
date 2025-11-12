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

	Snake WithDirection(const Vector2D& newDirection) const;

	Snake Move() const;
	Snake Grow() const;

	Vector2D GetHeadPosition() const { return segments.front()->GetPosition(); }
	const std::deque<std::unique_ptr<SnakeSegment>>& GetSegments() const { return segments; }
	int GetLength() const { return segments.size(); }
	Vector2D GetCurrentDirection() const { return currentDirection; }

	bool CheckSelfCollision() const;

private:
	std::unique_ptr<SnakeSegment> CreateNewHead() const;
	std::deque<std::unique_ptr<SnakeSegment>> CopySegments(const Snake& snake) const;
};