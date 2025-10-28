#include "console_renderer.cpp"
#include "game.hpp"


int main() {
    ConsoleRenderer renderer;
   

    Vector2D snakeStartPos = {GameConfig::FIELD_WIDTH / 2, GameConfig::FIELD_HEIGHT / 2};
    Snake snake = Snake(
        [SnakeSegment(snakeStartPos), SnakeSegment(snakeStartPos + Direction::LEFT)],
        Direction::RIGHT;
    )

    GameEngine game = GameEngine(snake);

    while (game.getState() != GameState::VICTORY || game.getState() != GameState::GAME_OVER)
    {

    }
}