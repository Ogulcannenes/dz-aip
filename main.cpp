#include <iostream>
#include <cstddef>
#include <new>

void freeMatrix(int ** m, size_t rows)
{
    for (size_t i = 0; i < rows; ++i)
    {
        delete[] m[i];
    }
    delete[] m;
}

int ** createMatrix(size_t rows, size_t cols)
{
    int ** m = new (std::nothrow) int *[rows];
    if (!m)
    {
        return nullptr;
    }
    for (size_t i = 0; i < rows; ++i)
    {
        m[i] = new (std::nothrow) int[cols];
        if (!m[i])
        {
            freeMatrix(m, i);
            return nullptr;
        }
    }   
    return m;
}

bool readMatrix(int ** m, size_t rows, size_t cols)
{
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            if (!(std::cin >> m[i][j]))
            {
                return false;
            }
        }
    }
    return true;
}

int ** transpose(int ** m, size_t rows, size_t cols)
{
    int ** t = createMatrix(cols, rows);
    if (!t)
    {
        return nullptr;
    }
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            t[j][i] = m[i][j];
        }
    }       
    return t;
}

void printMatrix(int ** m, size_t rows, size_t cols)
{
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            if (j > 0)
            {
                std::cout << ' ';
            }
            std::cout << m[i][j];
        }
        std::cout << '\n';
    }
}

int main()
{
    size_t rows = 0, cols = 0;
    std::cin >> rows >> cols;
    if (!std::cin)
    {
        std::cerr << "Invalid input\n";
        return 1;
    }

    int ** m = createMatrix(rows, cols);
    if (!m)
    {
        std::cerr << "Memory allocation failed\n";
        return 2;
    }

    if (!readMatrix(m, rows, cols))
    {
        std::cerr << "Invalid input\n";
        freeMatrix(m, rows);
        return 1;
    }

    int ** t = transpose(m, rows, cols);
    if (!t)
    {
        std::cerr << "Memory allocation failed\n";
        freeMatrix(m, rows);
        return 2;
    }

    printMatrix(t, cols, rows);
    freeMatrix(t, cols);
    freeMatrix(m, rows);
    return 0;
}