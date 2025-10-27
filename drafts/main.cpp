#include <iostream>
#include <vector>

#include "snake.hpp"
#include "field.hpp"
#include "render.hpp"
#include "config.hpp";


const std::pair<int, int> START_POSITION = {8, 8};

int main()
{
    Snake snake = Snake();
    Field field = Field(FIELD_WIDTH, FIELD_HEIGHT);
    
}