
#include <iostream>
#include <ostream>
#include <vector>

template<typename T>
std::ostream& operator <<(std::ostream& o, std::vector<T> vec)
{
    for (T i : vec)
    {
        o << i << ' ';
    }
    o << std::endl;
    return o;
}


int main()
{
    std::vector<std::vector<std::string>> matrix = {};
    std::vector<std::string> row = {"hi"};
    row.resize(50, row[0]);
    std::cout << matrix;

    for (int i = 0; i < 6; i++) {
        matrix.push_back(row);
    }

    matrix[1][1].replace(1, 1, "a");

    std::cout << matrix;

    return 0;
}