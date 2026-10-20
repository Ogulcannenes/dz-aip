#include <cstddef>
#include <iostream>
#include <limits>
#include <new>

namespace uzun
{
  constexpr int EXIT_INPUT_ERROR = 1;
  constexpr int EXIT_MEMORY_ERROR = 2;

  static void freeMatrix(int ** m, std::size_t rows)
  {
    for (std::size_t i = 0; i < rows; ++i)
    {
      delete[] m[i];
    }
    delete[] m;
  }

  static int ** createMatrix(std::size_t rows, std::size_t cols)
  {
    int ** m = nullptr;
    std::size_t i = 0;
    try
    {
      m = new int *[rows];
      for (; i < rows; ++i)
      {
        m[i] = new int[cols];
      }
    }
    catch (const std::bad_alloc &)
    {
      freeMatrix(m, i);
      return nullptr;
    }
    return m;
  }

  static bool readSize(std::size_t & value)
  {
    std::cin >> std::ws;
    if (std::cin.peek() == '-')
    {
      return false;
    }
    return static_cast< bool >(std::cin >> value);
  }

  static bool readMatrix(int * const * m, std::size_t rows, std::size_t cols)
  {
    for (std::size_t i = 0; i < rows; ++i)
    {
      for (std::size_t j = 0; j < cols; ++j)
      {
        if (!(std::cin >> m[i][j]))
        {
          return false;
        }
      }
    }
    return true;
  }

  static bool printMatrix(
      const int * const * m, std::size_t rows1, std::size_t cols1, std::size_t rows2, std::size_t cols2)
  {
    const std::size_t rows = rows1 > rows2 ? rows1 : rows2;
    for (std::size_t i = 0; i < rows; ++i)
    {
      const std::size_t leftCols = i < rows1 ? cols1 : 0;
      const std::size_t rightCols = i < rows2 ? cols2 : 0;
      for (std::size_t j = 0; j < leftCols + rightCols; ++j)
      {
        if (j > 0)
        {
          std::cout << ' ';
        }
        std::cout << m[i][j];
      }
      std::cout << '\n';
    }
    return static_cast< bool >(std::cout);
  }
}

int ** concat_rows(const int * const * lhs, std::size_t rows1, std::size_t cols1, const int * const * rhs,
    std::size_t rows2, std::size_t cols2);

int ** concat_rows(const int * const * lhs, std::size_t rows1, std::size_t cols1, const int * const * rhs,
    std::size_t rows2, std::size_t cols2)
{
  const std::size_t rows = rows1 > rows2 ? rows1 : rows2;
  int ** result = nullptr;
  std::size_t i = 0;
  try
  {
    result = new int *[rows];
    for (; i < rows; ++i)
    {
      const std::size_t leftCols = i < rows1 ? cols1 : 0;
      const std::size_t rightCols = i < rows2 ? cols2 : 0;
      if (leftCols > std::numeric_limits< std::size_t >::max() - rightCols)
      {
        throw std::bad_alloc();
      }
      result[i] = new int[leftCols + rightCols];
      for (std::size_t j = 0; j < leftCols; ++j)
      {
        result[i][j] = lhs[i][j];
      }
      for (std::size_t j = 0; j < rightCols; ++j)
      {
        result[i][leftCols + j] = rhs[i][j];
      }
    }
  }
  catch (const std::bad_alloc &)
  {
    uzun::freeMatrix(result, i);
    return nullptr;
  }
  return result;
}

int main()
{
  std::size_t rows1 = 0;
  std::size_t cols1 = 0;
  if (!uzun::readSize(rows1) || !uzun::readSize(cols1))
  {
    std::cerr << "Invalid input\n";
    return uzun::EXIT_INPUT_ERROR;
  }
  int ** const lhs = uzun::createMatrix(rows1, cols1);
  if (!lhs)
  {
    std::cerr << "Memory allocation failed\n";
    return uzun::EXIT_MEMORY_ERROR;
  }
  if (!uzun::readMatrix(lhs, rows1, cols1))
  {
    std::cerr << "Invalid input\n";
    uzun::freeMatrix(lhs, rows1);
    return uzun::EXIT_INPUT_ERROR;
  }

  std::size_t rows2 = 0;
  std::size_t cols2 = 0;
  if (!uzun::readSize(rows2) || !uzun::readSize(cols2))
  {
    std::cerr << "Invalid input\n";
    uzun::freeMatrix(lhs, rows1);
    return uzun::EXIT_INPUT_ERROR;
  }
  int ** const rhs = uzun::createMatrix(rows2, cols2);
  if (!rhs)
  {
    std::cerr << "Memory allocation failed\n";
    uzun::freeMatrix(lhs, rows1);
    return uzun::EXIT_MEMORY_ERROR;
  }
  const bool read = uzun::readMatrix(rhs, rows2, cols2);
  std::cin >> std::ws;
  if (!read || !std::cin.eof())
  {
    std::cerr << "Invalid input\n";
    uzun::freeMatrix(rhs, rows2);
    uzun::freeMatrix(lhs, rows1);
    return uzun::EXIT_INPUT_ERROR;
  }

  int ** const result = concat_rows(lhs, rows1, cols1, rhs, rows2, cols2);
  if (!result)
  {
    std::cerr << "Memory allocation failed\n";
    uzun::freeMatrix(rhs, rows2);
    uzun::freeMatrix(lhs, rows1);
    return uzun::EXIT_MEMORY_ERROR;
  }
  const bool printed = uzun::printMatrix(result, rows1, cols1, rows2, cols2);
  const std::size_t rows = rows1 > rows2 ? rows1 : rows2;
  uzun::freeMatrix(result, rows);
  uzun::freeMatrix(rhs, rows2);
  uzun::freeMatrix(lhs, rows1);
  if (!printed)
  {
    std::cerr << "Output failed\n";
    return uzun::EXIT_INPUT_ERROR;
  }
  return 0;
}
