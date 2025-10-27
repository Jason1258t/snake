#pragma once
#include "field.hpp"

class IRenderer
{
public:
	virtual ~IRenderer() = default;
	virtual bool initialize() = 0;
	virtual bool render(const GameField& field, int score) = 0;
	virtual bool showGameOver(int finalScore) = 0;
	virtual bool clear() = 0;
};