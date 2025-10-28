#include "console_renderer.hpp"
#include "domain/game_engine.hpp"

int main()
{
	ConsoleRenderer renderer;

	std::deque<std::unique_ptr<SnakeSegment>> segments;

	Vector2D startPos = Vector2D{ GameConfig::FIELD_HEIGHT / 2, GameConfig::FIELD_WIDTH / 2 };

	segments.push_back(std::make_unique<SnakeSegment>(startPos));
	segments.push_back(std::make_unique<SnakeSegment>(startPos + Direction::LEFT));

	Snake snake = Snake(segments, Direction::RIGHT);

	GameField field = GameField(GameConfig::FIELD_WIDTH, GameConfig::FIELD_HEIGHT);
	GameEngine engine = GameEngine(snake, field);

	InputManager::InputManager input;
	engine.registerInputManager(input);
}