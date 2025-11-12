#include "Snake.hpp"
#include "../utils/direction_utils.hpp"
#include <algorithm>

Snake::Snake(std::deque<std::unique_ptr<SnakeSegment>>&& initSegments,
	const Vector2D& initialDirection) : segments(std::move(initSegments)), currentDirection(initialDirection) {}

Snake::Snake(const Snake& other) : currentDirection(other.currentDirection)
{
	segments = CopySegments(other);
}

Snake& Snake::operator=(const Snake& other)
{
	if (this != &other)
	{
		currentDirection = other.currentDirection;
		segments = CopySegments(other);
	}
	return *this;
}

Snake Snake::WithDirection(const Vector2D& newDirection) const
{
	if (Direction::isOppositeDirection(currentDirection, newDirection) || !Direction::isDirectionValid(newDirection))
		return *this;

	return Snake(CopySegments(*this), newDirection);
}

Snake Snake::Move() const
{
	auto newSegments = CopySegments(*this);
	newSegments.push_front(CreateNewHead());
	newSegments.pop_back();
	return Snake(std::move(newSegments), currentDirection);
}

Snake Snake::Grow() const
{
	auto newSegments = CopySegments(*this);
	newSegments.push_front(CreateNewHead());
	return Snake(std::move(newSegments), currentDirection);
}

std::unique_ptr<SnakeSegment> Snake::CreateNewHead() const
{
	return std::make_unique<SnakeSegment>(segments.front()->GetPosition() + currentDirection);
}

std::deque<std::unique_ptr<SnakeSegment>> Snake::CopySegments(const Snake& snake) const
{
	std::deque<std::unique_ptr<SnakeSegment>> newSegments;
	for (const auto& segment : snake.GetSegments())
	{
		newSegments.push_back(std::make_unique<SnakeSegment>(*segment));
	}
	return newSegments;
}

bool Snake::CheckSelfCollision() const
{
	const Vector2D& head = segments.front()->GetPosition();
	for (size_t i = 1; i < segments.size(); ++i)
	{
		if (head == segments[i]->GetPosition())
			return true;
	}
	return false;
}