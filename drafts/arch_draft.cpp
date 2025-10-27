#include <chrono>
#include <conio.h> // для _kbhit() и _getch() на Windows
#include <deque>
#include <iostream>
#include <memory>
#include <random>
#include <thread>
#include <vector>

// ============================================================================
// КОНСТАНТЫ
// ============================================================================
namespace GameConstants
{
constexpr int FIELD_WIDTH = 20;
constexpr int FIELD_HEIGHT = 15;
constexpr int INITIAL_SNAKE_LENGTH = 3;
constexpr int GAME_SPEED_MS = 150;
constexpr int APPLE_SCORE_VALUE = 10;
} // namespace GameConstants

// ============================================================================
// БАЗОВЫЕ СТРУКТУРЫ
// ============================================================================
struct Vector2D
{
	int x;
	int y;

	Vector2D operator+(const Vector2D& other) const
	{
		return { x + other.x, y + other.y };
	}

	Vector2D operator-(const Vector2D& other) const
	{
		return { x - other.x, y - other.y };
	}

	bool operator==(const Vector2D& other) const
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(const Vector2D& other) const
	{
		return !(*this == other);
	}

	Vector2D operator-() const
	{
		return { -x, -y };
	}
};

// Направления как векторы
namespace Direction
{
constexpr Vector2D UP = { 0, -1 };
constexpr Vector2D DOWN = { 0, 1 };
constexpr Vector2D LEFT = { -1, 0 };
constexpr Vector2D RIGHT = { 1, 0 };
constexpr Vector2D NONE = { 0, 0 };
} // namespace Direction

enum class GameState
{
	RUNNING,
	GAME_OVER,
	VICTORY
};

// ============================================================================
// ИГРОВЫЕ ОБЪЕКТЫ
// ============================================================================
enum class GameObjectType
{
	EMPTY,
	SNAKE_HEAD,
	SNAKE_BODY,
	APPLE
};

class GameObject
{
protected:
	Vector2D position;
	GameObjectType type;

public:
	GameObject(const Vector2D& pos, GameObjectType objType)
		: position(pos)
		, type(objType)
	{
	}

	virtual ~GameObject() = default;

	Vector2D getPosition() const { return position; }
	GameObjectType getType() const { return type; }

	bool setPosition(const Vector2D& newPos)
	{
		position = newPos;
		return true;
	}

	virtual char getDisplayChar() const = 0;
};

class SnakeSegment : public GameObject
{
private:
	bool isHead;

public:
	SnakeSegment(const Vector2D& pos, bool head = false)
		: GameObject(pos, head ? GameObjectType::SNAKE_HEAD : GameObjectType::SNAKE_BODY)
		, isHead(head)
	{
	}

	bool setAsHead(bool head)
	{
		isHead = head;
		type = head ? GameObjectType::SNAKE_HEAD : GameObjectType::SNAKE_BODY;
		return true;
	}

	bool getIsHead() const { return isHead; }

	char getDisplayChar() const override
	{
		return isHead ? 'O' : 'o';
	}
};

class Apple : public GameObject
{
public:
	explicit Apple(const Vector2D& pos)
		: GameObject(pos, GameObjectType::APPLE)
	{
	}

	char getDisplayChar() const override
	{
		return '@';
	}
};

// ============================================================================
// ИГРОВОЕ ПОЛЕ
// ============================================================================
class GameField
{
private:
	int width;
	int height;
	std::vector<std::vector<GameObject*>> grid;

public:
	GameField(int w, int h)
		: width(w)
		, height(h)
	{
		grid.resize(height, std::vector<GameObject*>(width, nullptr));
	}

	int getWidth() const { return width; }
	int getHeight() const { return height; }

	bool isPositionValid(const Vector2D& pos) const
	{
		return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
	}

	bool isPositionEmpty(const Vector2D& pos) const
	{
		if (!isPositionValid(pos))
			return false;
		return grid[pos.y][pos.x] == nullptr;
	}

	GameObject* getObjectAt(const Vector2D& pos) const
	{
		if (!isPositionValid(pos))
			return nullptr;
		return grid[pos.y][pos.x];
	}

	bool placeObject(GameObject* obj)
	{
		if (!obj || !isPositionValid(obj->getPosition()))
		{
			return false;
		}
		Vector2D pos = obj->getPosition();
		grid[pos.y][pos.x] = obj;
		return true;
	}

	bool removeObject(const Vector2D& pos)
	{
		if (!isPositionValid(pos))
			return false;
		grid[pos.y][pos.x] = nullptr;
		return true;
	}

	bool clearField()
	{
		for (auto& row : grid)
		{
			for (auto& cell : row)
			{
				cell = nullptr;
			}
		}
		return true;
	}

	const std::vector<std::vector<GameObject*>>& getGrid() const
	{
		return grid;
	}
};

// ============================================================================
// ИНТЕРФЕЙС ДЛЯ ОТРИСОВКИ
// ============================================================================
class IRenderer
{
public:
	virtual ~IRenderer() = default;
	virtual bool initialize() = 0;
	virtual bool render(const GameField& field, int score) = 0;
	virtual bool showGameOver(int finalScore) = 0;
	virtual bool clear() = 0;
};

// ============================================================================
// КОНСОЛЬНЫЙ РЕНДЕРЕР
// ============================================================================
class ConsoleRenderer : public IRenderer
{
public:
	bool initialize() override
	{
		system("cls");
		return true;
	}

	bool render(const GameField& field, int score) override
	{
		system("cls");

		std::cout << "SNAKE GAME | Score: " << score << "\n";
		std::cout << std::string(field.getWidth() + 2, '#') << "\n";

		const auto& grid = field.getGrid();
		for (const auto& row : grid)
		{
			std::cout << '#';
			for (const auto* obj : row)
			{
				if (obj)
				{
					std::cout << obj->getDisplayChar();
				}
				else
				{
					std::cout << ' ';
				}
			}
			std::cout << "#\n";
		}

		std::cout << std::string(field.getWidth() + 2, '#') << "\n";
		std::cout << "Controls: W/A/S/D or Arrows | Q - Quit\n";

		return true;
	}

	bool showGameOver(int finalScore) override
	{
		std::cout << "\n=== GAME OVER ===\n";
		std::cout << "Final Score: " << finalScore << "\n";
		std::cout << "Press any key to exit...\n";
		return true;
	}

	bool clear() override
	{
		system("cls");
		return true;
	}
};

// ============================================================================
// ГЕНЕРАТОР СЛУЧАЙНЫХ ПОЗИЦИЙ
// ============================================================================
class RandomGenerator
{
private:
	std::mt19937 generator;

public:
	RandomGenerator()
		: generator(std::random_device{}())
	{
	}

	Vector2D generatePosition(const GameField& field, const std::deque<Vector2D>& excludePositions)
	{
		std::uniform_int_distribution<int> distX(0, field.getWidth() - 1);
		std::uniform_int_distribution<int> distY(0, field.getHeight() - 1);

		Vector2D candidate;
		bool positionValid = false;

		while (!positionValid)
		{
			candidate.x = distX(generator);
			candidate.y = distY(generator);

			positionValid = isPositionValid(candidate, excludePositions) && field.isPositionEmpty(candidate);
		}

		return candidate;
	}

private:
	static bool isPositionValid(const Vector2D& pos, const std::deque<Vector2D>& excludePositions)
	{
		for (const auto& excluded : excludePositions)
		{
			if (pos == excluded)
			{
				return false;
			}
		}
		return true;
	}
};

// ============================================================================
// ЗМЕЙКА
// ============================================================================
class Snake
{
private:
	std::deque<Vector2D> bodyPositions;
	std::deque<std::unique_ptr<SnakeSegment>> segments;
	Vector2D currentDirection;
	Vector2D nextDirection;
	bool growing;

public:
	Snake(const Vector2D& startPosition, const Vector2D& initialDirection)
		: currentDirection(initialDirection)
		, nextDirection(initialDirection)
		, growing(false)
	{
		bodyPositions.push_back(startPosition);
		segments.push_back(std::make_unique<SnakeSegment>(startPosition, true));
	}

	bool setDirection(const Vector2D& newDirection)
	{
		// Проверка на противоположное направление
		if (isOppositeDirection(currentDirection, newDirection))
		{
			return false;
		}
		// Проверка что это допустимое направление
		if (newDirection != Direction::UP && newDirection != Direction::DOWN && newDirection != Direction::LEFT && newDirection != Direction::RIGHT)
		{
			return false;
		}
		nextDirection = newDirection;
		return true;
	}

	Vector2D getHeadPosition() const
	{
		return bodyPositions.front();
	}

	const std::deque<Vector2D>& getBodyPositions() const
	{
		return bodyPositions;
	}

	const std::deque<std::unique_ptr<SnakeSegment>>& getSegments() const
	{
		return segments;
	}

	int getLength() const
	{
		return static_cast<int>(bodyPositions.size());
	}

	Vector2D getCurrentDirection() const
	{
		return currentDirection;
	}

	bool move()
	{
		currentDirection = nextDirection;
		Vector2D newHeadPos = bodyPositions.front() + currentDirection;

		bodyPositions.push_front(newHeadPos);

		// Обновляем старую голову на тело
		if (segments.size() > 0)
		{
			segments[0]->setAsHead(false);
		}

		// Создаём новую голову
		segments.push_front(std::make_unique<SnakeSegment>(newHeadPos, true));

		if (!growing)
		{
			bodyPositions.pop_back();
			segments.pop_back();
		}
		else
		{
			growing = false;
		}

		// Обновляем позиции всех сегментов
		for (size_t i = 0; i < segments.size(); ++i)
		{
			segments[i]->setPosition(bodyPositions[i]);
		}

		return true;
	}

	bool grow()
	{
		growing = true;
		return true;
	}

	bool checkSelfCollision() const
	{
		const Vector2D& head = bodyPositions.front();
		for (size_t i = 1; i < bodyPositions.size(); ++i)
		{
			if (head == bodyPositions[i])
			{
				return true;
			}
		}
		return false;
	}

private:
	static bool isOppositeDirection(const Vector2D& current, const Vector2D& next)
	{
		return (current.x + next.x == 0 && current.y + next.y == 0);
	}
};

// ============================================================================
// ИГРОВОЙ ДВИЖОК
// ============================================================================
class GameEngine
{
private:
	GameField field;
	Snake snake;
	std::unique_ptr<Apple> apple;
	RandomGenerator randomGen;
	GameState state;
	int score;

public:
	GameEngine()
		: field(GameConstants::FIELD_WIDTH, GameConstants::FIELD_HEIGHT)
		, snake(Vector2D{ GameConstants::FIELD_WIDTH / 2, GameConstants::FIELD_HEIGHT / 2 }, Direction::RIGHT)
		, state(GameState::RUNNING)
		, score(0)
	{
		spawnApple();
		updateField();
	}

	bool processInput(char input)
	{
		Vector2D newDirection = getDirectionFromInput(input);
		if (newDirection != Direction::NONE)
		{
			return snake.setDirection(newDirection);
		}
		return true;
	}

	bool update()
	{
		if (state != GameState::RUNNING)
		{
			return false;
		}

		snake.move();

		if (checkWallCollision() || snake.checkSelfCollision())
		{
			state = GameState::GAME_OVER;
			return false;
		}

		if (checkAppleCollision())
		{
			snake.grow();
			score += GameConstants::APPLE_SCORE_VALUE;
			spawnApple();
		}

		updateField();
		return true;
	}

	const GameField& getField() const
	{
		return field;
	}

	int getScore() const
	{
		return score;
	}

	GameState getState() const
	{
		return state;
	}

private:
	bool spawnApple()
	{
		Vector2D applePos = randomGen.generatePosition(field, snake.getBodyPositions());
		apple = std::make_unique<Apple>(applePos);
		return true;
	}

	bool updateField()
	{
		field.clearField();

		// Размещаем яблоко
		if (apple)
		{
			field.placeObject(apple.get());
		}

		// Размещаем змейку
		const auto& segments = snake.getSegments();
		for (const auto& segment : segments)
		{
			field.placeObject(segment.get());
		}

		return true;
	}

	bool checkWallCollision() const
	{
		Vector2D head = snake.getHeadPosition();
		return !field.isPositionValid(head);
	}

	bool checkAppleCollision() const
	{
		if (!apple)
			return false;
		return snake.getHeadPosition() == apple->getPosition();
	}

	static Vector2D getDirectionFromInput(char input)
	{
		switch (input)
		{
		case 'w':
		case 'W':
		case 72:
			return Direction::UP;
		case 's':
		case 'S':
		case 80:
			return Direction::DOWN;
		case 'a':
		case 'A':
		case 75:
			return Direction::LEFT;
		case 'd':
		case 'D':
		case 77:
			return Direction::RIGHT;
		default:
			return Direction::NONE;
		}
	}
};

// ============================================================================
// ИГРОВОЙ ЦИКЛ
// ============================================================================
class Game
{
private:
	GameEngine engine;
	IRenderer* renderer;
	bool running;

public:
	explicit Game(IRenderer* renderInstance)
		: renderer(renderInstance)
		, running(true)
	{
	}

	bool initialize()
	{
		return renderer->initialize();
	}

	bool run()
	{
		while (running && engine.getState() == GameState::RUNNING)
		{
			handleInput();
			engine.update();
			renderer->render(engine.getField(), engine.getScore());
			std::this_thread::sleep_for(std::chrono::milliseconds(GameConstants::GAME_SPEED_MS));
		}

		if (engine.getState() == GameState::GAME_OVER)
		{
			renderer->showGameOver(engine.getScore());
			_getch();
		}

		return true;
	}

private:
	bool handleInput()
	{
		if (_kbhit())
		{
			char key = _getch();
			if (key == 'q' || key == 'Q')
			{
				running = false;
				return false;
			}
			if (key == 0 || key == -32)
			{
				key = _getch();
			}
			engine.processInput(key);
		}
		return true;
	}
};

// ============================================================================
// ТОЧКА ВХОДА
// ============================================================================
int main()
{
	ConsoleRenderer renderer;
	Game game(&renderer);

	if (!game.initialize())
	{
		std::cerr << "Failed to initialize game\n";
		return 1;
	}

	game.run();

	return 0;
}