#pragma once
#include <unordered_map>
#include "../objects/GameObject.hpp"

class ConsoleRendererMapper
{
private:
    std::unordered_map<GameObjectType, char> symbolMap;

public:
    ConsoleRendererMapper()
    {
        symbolMap = {
            {GameObjectType::EMPTY, ' '},
            {GameObjectType::SNAKE_BODY, 'O'},
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