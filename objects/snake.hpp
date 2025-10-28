#pragma once

#include "game_object.hpp"
#include "../utils/vector_2d.hpp"
#include <deque>
#include <memory>

class Snake
{
private:
    std::deque<std::unique_ptr<SnakeSegment>> segments;
    Vector2D currentDirection;

public:
   Snake(std::deque<std::unique_ptr<SnakeSegment>>&& initSegments, const Vector2D& initialDirection)
    : segments(std::move(initSegments)), currentDirection(initialDirection) {}

    bool setDirection(const Vector2D& newDirection)
    {
        if (Direction::isOppositeDirection(currentDirection, newDirection))
            return false;
        if (!Direction::isDirectionValid(newDirection))
            return false;
        currentDirection = newDirection;
        return true;
    }

    Vector2D getHeadPosition() const 
    { 
        return segments.front()->getPosition(); 
    }

    const std::deque<std::unique_ptr<SnakeSegment>>& getSegments() const 
    { 
        return segments; 
    }

    int getLength() const 
    { 
        return segments.size(); 
    }
    
    Vector2D getCurrentDirection() const 
    { 
        return currentDirection; 
    }

    bool move()
    {
        Vector2D newHeadPos = segments.front()->getPosition() + currentDirection;
        segments.push_front(std::make_unique<SnakeSegment>(newHeadPos));
        segments.pop_back(); 
        return true;
    }

    bool grow()
    {
        Vector2D newHeadPos = segments.front()->getPosition() + currentDirection;
        segments.push_front(std::make_unique<SnakeSegment>(newHeadPos));
        return true;
    }

    bool checkSelfCollision() const
    {
        const Vector2D& head = segments.front()->getPosition();
        for (size_t i = 1; i < segments.size(); ++i)
        {
            if (head == segments[i]->getPosition())
                return true;
        }
        return false;
    }
};