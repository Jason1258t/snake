#include "console_renderer.hpp"
#include <ncurses.h>
#include <string>

constexpr int TOTAL_BORDER_PADDING = 2;
constexpr char BORDER_CHAR = '#';

bool ConsoleRenderer::initialize()
{
    // Уже инициализировано в InputManager
    return true;
}

bool ConsoleRenderer::render(const GameField& field, int score)
{
    clear();

    // Верхняя строка с счетом
    std::string scoreStr = "SNAKE GAME | Score: " + std::to_string(score);
    mvprintw(0, 0, "%s", scoreStr.c_str());

    // Верхняя граница
    std::string topBorder(field.getWidth() + TOTAL_BORDER_PADDING, BORDER_CHAR);
    mvprintw(1, 0, "%s", topBorder.c_str());

    // Игровое поле
    const auto& grid = field.getGrid();
    for (int y = 0; y < grid.size(); y++)
    {
        mvprintw(y + 2, 0, "%c", BORDER_CHAR); // Левая граница
        
        for (int x = 0; x < grid[y].size(); x++)
        {
            char displayChar = mapper.getDisplayChar(grid[y][x]->getType());
            mvaddch(y + 2, x + 1, displayChar);
        }
        
        mvprintw(y + 2, field.getWidth() + 1, "%c", BORDER_CHAR); // Правая граница
    }

    // Нижняя граница
    std::string bottomBorder(field.getWidth() + TOTAL_BORDER_PADDING, BORDER_CHAR);
    mvprintw(grid.size() + 2, 0, "%s", bottomBorder.c_str());

    // Управление
    mvprintw(grid.size() + 3, 0, "Controls: W/A/S/D  | ESC - Pause");

    refresh();
    return true;
}

bool ConsoleRenderer::showGameOver(int finalScore)
{
    clear();
    
    int height, width;
    getmaxyx(stdscr, height, width);
    
    int y = height / 2 - 2;
    
    mvprintw(y++, width/2 - 7, "=== GAME OVER ===");
    mvprintw(y++, width/2 - 8, "Final Score: %d", finalScore);
    mvprintw(y++, width/2 - 12, "Press any key to exit...");
    
    refresh();
    return true;
}

bool ConsoleRenderer::clear()
{
    ::clear(); // Вызов функции ncurses
    return true;
}