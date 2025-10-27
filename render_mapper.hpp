#pragma once
#include <unordered_map>
#include "game_object.hpp"

class ConsoleRendererMapper
{
private:
    std::unordered_map<GameObjectType, char> symbolMap;

public:
    ConsoleRendererMapper()
    {
        symbolMap = {
            {GameObjectType::EMPTY, ' '},
            {GameObjectType::SNAKE_HEAD, 'O'},
            {GameObjectType::SNAKE_BODY, 'o'},
            {GameObjectType::APPLE, '@'}
        };
    }

    char getDisplayChar(GameObjectType type) const
    {
        auto it = symbolMap.find(type);
        if (it != symbolMap.end()) {
            return it->second;
        }
        return '?';
    }

    void setSymbol(GameObjectType type, char symbol)
    {
        symbolMap[type] = symbol;
    }
};