#pragma once
#include "Vector2D.hpp"

namespace Direction
{
	static bool isDirectionValid(const Vector2D& direction)
	{
		return direction == Direction::UP || direction == Direction::DOWN || direction == Direction::LEFT || direction == Direction::RIGHT;
	}

	static bool isOppositeDirection(const Vector2D& current, const Vector2D& next)
	{
		return (current + next == Vector2D{ 0, 0 });
	}
} // namespace Direction