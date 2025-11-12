#pragma once

#include "ConsoleRendererMapper.hpp"
#include "IRenderer.hpp"

class ConsoleRenderer : public IRenderer
{
private:
	ConsoleRendererMapper mapper;

public:
	bool initialize() override;
	bool render(const GameField& field, int score) override;
	bool showGameOver(int finalScore) override;
	bool clear() override;
};