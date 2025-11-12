#pragma once

#include "ConsoleRendererMapper.hpp"
#include "IRenderer.hpp"

class ConsoleRenderer : public IRenderer
{
private:
	ConsoleRendererMapper mapper;

public:
	bool Initialize() override;
	bool Render(const GameField& field, int score) override;
	bool ShowGameOver(int finalScore) override;
	bool Clear() override;
};