#include "domain/GameEngine.hpp"
#include "render/ConsoleRenderer.hpp"
#include <thread>

int main()
{
	ConsoleRenderer renderer;
	GameField field = GameField(GameConfig::FIELD_WIDTH, GameConfig::FIELD_HEIGHT);
	GameEngine engine = GameEngine(std::move(field));

	InputManager::InputManager input;
	engine.RegisterInputManager(input);

	while (engine.GetState() == GameState::RUNNING || engine.GetState() == GameState::PAUSE)
	{
		input.HandleInput();
		engine.Update();
		renderer.Render(engine.GetField(), engine.GetScore());
		std::this_thread::sleep_for(std::chrono::milliseconds(GameConfig::TICK_DURATION));
	}

	if (engine.GetState() == GameState::GAME_OVER)
		renderer.ShowGameOver(engine.GetScore());

	while (true)
		;
}