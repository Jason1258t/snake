#pragma once

struct Vector2D
{
	int x;
	int y;

	Vector2D operator+(const Vector2D& other) const
	{
		return { x + other.x, y + other.y };
	}

	Vector2D operator-(const Vector2D& other) const
	{
		return { x - other.x, y - other.y };
	}

	bool operator==(const Vector2D& other) const
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(const Vector2D& other) const
	{
		return !(*this == other);
	}

	Vector2D operator-() const
	{
		return { -x, -y };
	}
};

namespace Direction
{
	constexpr Vector2D UP = { 0, -1 };
	constexpr Vector2D DOWN = { 0, 1 };
	constexpr Vector2D LEFT = { -1, 0 };
	constexpr Vector2D RIGHT = { 1, 0 };
	constexpr Vector2D NONE = { 0, 0 };
}