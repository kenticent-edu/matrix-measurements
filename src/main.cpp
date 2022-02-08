#include "Matrix.h"

#include <iostream>

using namespace std;

int main()
{
    Matrix<int> m{
        {1, 2, 3},
        {4, 5, 6},
    };
    Matrix<int> m1{
        {7, 8},
        {9, 10},
        {11, 12},
    };
    Matrix<int> m2{
        {1, 2, 3, 4},
        {3, 2, 4, 5},
        {3, 3, 5, 7},
    };
    cout << m2.sumByRows() << m2.sumByColumns() << m * m1;
    auto m3 = Matrix<int>::random(1000, 1000);
    auto m4 = Matrix<int>::random(1000, 1000);
    // Don't print the results, just measure the times
    auto m5 = m3 * m4;
    auto m6 = m3 + m4;
    auto m7 = m3 - m4;
    auto m8 = m3.sumByRows();
    auto m9 = m3.sumByColumns();
    auto m10 = m3.transpose();

    return 0;
}
