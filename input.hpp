#pragma once
#include "vector_2d.hpp"
#include <cctype>

namespace InputMapper
{
	static Vector2D getDirectionFromInput(char input)
	{
		switch (tolower(input))
		{
		case 'w':
		case 72: // arrow values
			return Direction::UP;
		case 's':
		case 80:
			return Direction::DOWN;
		case 'a':
		case 75:
			return Direction::LEFT;
		case 'd':
		case 77:
			return Direction::RIGHT;
		default:
			return Direction::NONE;
		}
	}
} // namespace InputMapper