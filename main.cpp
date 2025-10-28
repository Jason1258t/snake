#include "domain/game_engine.hpp"
#include "render/console_renderer.hpp"
#include <thread>

int main()
{
	ConsoleRenderer renderer;

	std::deque<std::unique_ptr<SnakeSegment>> segments;

	Vector2D startPos = Vector2D{ GameConfig::FIELD_HEIGHT / 2, GameConfig::FIELD_WIDTH / 2 };

	segments.push_back(std::make_unique<SnakeSegment>(startPos));
	segments.push_back(std::make_unique<SnakeSegment>(startPos + Direction::LEFT));

	Snake snake = Snake(std::move(segments), Direction::RIGHT);

	GameField field = GameField(GameConfig::FIELD_WIDTH, GameConfig::FIELD_HEIGHT);
	GameEngine engine = GameEngine(std::move(snake), std::move(field));

	InputManager::InputManager input;
	engine.registerInputManager(input);

	while (engine.getState() == GameState::RUNNING || engine.getState() == GameState::PAUSE)
	{
		input.handleInput();
		engine.update();
		renderer.render(engine.getField(), engine.getScore());
		std::this_thread::sleep_for(std::chrono::milliseconds(GameConfig::TICK_DURATION));
	}

	if (engine.getState() == GameState::GAME_OVER)
		renderer.showGameOver(engine.getScore());

	_getch();
}