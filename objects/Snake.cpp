#include "Snake.hpp"
#include "../utils/direction_utils.hpp"
#include <algorithm>

Snake::Snake(std::deque<std::unique_ptr<SnakeSegment>>&& initSegments,
	const Vector2D& initialDirection) : segments(std::move(initSegments)), currentDirection(initialDirection) {}

Snake::Snake(const Snake& other) : currentDirection(other.currentDirection)
{
	for (const auto& segment : other.segments)
	{
		segments.push_back(std::make_unique<SnakeSegment>(*segment));
	}
}

Snake& Snake::operator=(const Snake& other)
{
	if (this != &other)
	{
		currentDirection = other.currentDirection;

		std::deque<std::unique_ptr<SnakeSegment>> newSegments;
		for (const auto& segment : other.segments)
		{
			newSegments.push_back(std::make_unique<SnakeSegment>(*segment));
		}
		segments = std::move(newSegments);
	}
	return *this;
}

Snake Snake::withDirection(const Vector2D& newDirection) const
{
	if (Direction::isOppositeDirection(currentDirection, newDirection) || !Direction::isDirectionValid(newDirection))
		return *this;

	return Snake(copySegments(), newDirection);
}

Snake Snake::move() const
{
	auto newSegments = copySegments();
	newSegments.push_front(createNewHead());
	newSegments.pop_back();
	return Snake(std::move(newSegments), currentDirection);
}

Snake Snake::grow() const
{
	auto newSegments = copySegments();
	newSegments.push_front(createNewHead());
	return Snake(std::move(newSegments), currentDirection);
}

Vector2D Snake::calculateNewHeadPosition() const
{
	return segments.front()->GetPosition() + currentDirection;
}

std::unique_ptr<SnakeSegment> Snake::createNewHead() const
{
	return std::make_unique<SnakeSegment>(calculateNewHeadPosition());
}

std::deque<std::unique_ptr<SnakeSegment>> Snake::copySegments() const
{
	std::deque<std::unique_ptr<SnakeSegment>> newSegments;
	for (const auto& segment : segments)
	{
		newSegments.push_back(std::make_unique<SnakeSegment>(*segment));
	}
	return newSegments;
}

bool Snake::checkSelfCollision() const
{
	const Vector2D& head = segments.front()->GetPosition();
	for (size_t i = 1; i < segments.size(); ++i)
	{
		if (head == segments[i]->GetPosition())
			return true;
	}
	return false;
}