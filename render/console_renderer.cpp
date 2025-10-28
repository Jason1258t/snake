#include "console_renderer.hpp"
#include <iostream>

constexpr int TOTAL_BORDER_PADDING = 2;
constexpr char BORDER_CHAR = '#';

bool ConsoleRenderer::initialize()
{
    system("cls");
    return true;
}

bool ConsoleRenderer::render(const GameField& field, int score)
{
    system("cls");

    std::cout << "SNAKE GAME | Score: " << score << "\n";
    std::cout << std::string(field.getWidth() + TOTAL_BORDER_PADDING, BORDER_CHAR) << "\n";

    const auto& grid = field.getGrid();
    for (const auto& row : grid)
    {
        std::cout << BORDER_CHAR;
        for (const auto& obj : row)
        {
            std::cout << mapper.getDisplayChar(obj->getType());
        }
        std::cout << BORDER_CHAR << "\n";
    }

    std::cout << std::string(field.getWidth() + TOTAL_BORDER_PADDING, BORDER_CHAR) << "\n";
    std::cout << "Controls: W/A/S/D  | ESC - Pause\n";

    return true;
}

bool ConsoleRenderer::showGameOver(int finalScore)
{
    std::cout << "\n=== GAME OVER ===\n";
    std::cout << "Final Score: " << finalScore << "\n";
    std::cout << "Press any key to exit...\n";
    return true;
}

bool ConsoleRenderer::clear()
{
    system("cls");
    return true;
}