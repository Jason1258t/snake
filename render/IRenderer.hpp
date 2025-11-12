#pragma once
#include "../objects/GameField.hpp"

class IRenderer
{
public:
	virtual ~IRenderer() = default;
	virtual bool Initialize() = 0;
	virtual bool Render(const GameField& field, int score) = 0;
	virtual bool ShowGameOver(int finalScore) = 0;
	virtual bool Clear() = 0;
};