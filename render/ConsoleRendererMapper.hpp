#pragma once
#include "../objects/GameObject.hpp"
#include <unordered_map>

class ConsoleRendererMapper
{
private:
	std::unordered_map<GameObjectType, char> symbolMap;

public:
	ConsoleRendererMapper()
	{
		symbolMap = {
			{ GameObjectType::EMPTY, ' ' },
			{ GameObjectType::SNAKE_BODY, 'O' },
			{ GameObjectType::APPLE, '@' }
		};
	}

	char GetDisplayChar(GameObjectType type) const
	{
		auto item = symbolMap.find(type);
		if (item != symbolMap.end())
		{
			return item->second;
		}
		return '?';
	}

	void SetSymbol(GameObjectType type, char symbol)
	{
		symbolMap[type] = symbol;
	}
};