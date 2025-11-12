#include "ConsoleRenderer.hpp"
#include <ncurses.h>
#include <string>

constexpr int TOTAL_BORDER_PADDING = 2;
constexpr char BORDER_CHAR = '#';

bool ConsoleRenderer::initialize()
{
	return true;
}

bool ConsoleRenderer::render(const GameField& field, int score)
{
	clear();

	std::string scoreStr = "SNAKE GAME | Score: " + std::to_string(score);
	mvprintw(0, 0, "%s", scoreStr.c_str());

	std::string topBorder(field.getWidth() + TOTAL_BORDER_PADDING, BORDER_CHAR);
	mvprintw(1, 0, "%s", topBorder.c_str());

	const int fieldTopOffset = 2;
	const int fieldLeftOffset = 1;

	const auto& grid = field.getGrid();
	for (int y = 0; y < static_cast<int>(grid.size()); y++)
	{
		mvprintw(y + fieldTopOffset, 0, "%c", BORDER_CHAR);

		for (int x = 0; x < static_cast<int>(grid[y].size()); x++)
		{
			char displayChar = mapper.getDisplayChar(grid[y][x]->GetType());
			mvaddch(y + fieldTopOffset, x + fieldLeftOffset, displayChar);
		}

		mvprintw(y + fieldTopOffset, field.getWidth() + fieldLeftOffset, "%c", BORDER_CHAR);
	}

	std::string bottomBorder(field.getWidth() + TOTAL_BORDER_PADDING, BORDER_CHAR);

	mvprintw(grid.size() + fieldTopOffset, 0, "%s", bottomBorder.c_str());
	mvprintw(grid.size() + fieldTopOffset + 1, 0, "Controls: W/A/S/D  | ESC - Pause");

	refresh();
	return true;
}

bool ConsoleRenderer::showGameOver(int finalScore)
{
	clear();

	int height, width;
	getmaxyx(stdscr, height, width);

	int y = height / 2 - 2;

	mvprintw(y++, width / 2 - 7, "=== GAME OVER ===");
	mvprintw(y++, width / 2 - 8, "Final Score: %d", finalScore);

	refresh();
	return true;
}

bool ConsoleRenderer::clear()
{
	::clear();
	return true;
}