#ifndef MATRIX_MATRIX_H
#define MATRIX_MATRIX_H


#include "MeasureExecution.h"

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>

template<typename T>
class Matrix {
public:
    Matrix(size_t rowsAmount, size_t columnsAmount) : _rowsAmount(rowsAmount),
        _columnsAmount(columnsAmount),
        _matrix(new T[_rowsAmount*_columnsAmount]) {}

    Matrix(std::initializer_list<std::initializer_list<T>> initList) :
        _rowsAmount(initList.size()), _columnsAmount(initList.begin()->size()),
        _matrix(new T[_rowsAmount*_columnsAmount])
    {
        size_t i = 0;
        for (const auto& row : initList)
            for (const auto& elem : row) {
                _matrix[i] = elem;
                ++i;
            }
    }

    static Matrix<int> random(size_t rowsAmount, size_t columnsAmount)
    {
        Matrix m(rowsAmount, columnsAmount);
        std::default_random_engine re{};
        std::uniform_int_distribution<> negativeNineToNine(-9, 9);
        auto myRand = [&](){ return negativeNineToNine(re); };
        for (size_t i = 0; i < rowsAmount*columnsAmount; ++i)
            m._matrix[i] = myRand();
        return m;
    }

    Matrix(const Matrix& rhs) : _rowsAmount(rhs._rowsAmount),
        _columnsAmount(rhs._columnsAmount),
        _matrix(new T[_rowsAmount*_columnsAmount])
    {
        copyElementsFrom(rhs);
    }

    Matrix& operator=(const Matrix& rhs)
    {
        if (this == &rhs) return *this;

        if (rhs._rowsAmount == _rowsAmount &&
            rhs._columnsAmount == _columnsAmount) {
            copyElementsFrom(rhs);
            return *this;
        }

        _rowsAmount = rhs._rowsAmount;
        _columnsAmount = rhs._columnsAmount;
        delete[] _matrix;
        _matrix = new T[_rowsAmount*_columnsAmount];
        copyElementsFrom(rhs);
        return *this;
    }

    Matrix(Matrix&& rhs) noexcept : _rowsAmount(rhs._rowsAmount),
        _columnsAmount(rhs._columnsAmount), _matrix(rhs._matrix)
    {
        rhs._rowsAmount = 0;
        rhs._columnsAmount = 0;
        rhs._matrix = nullptr;
    }

    Matrix& operator=(Matrix&& rhs) noexcept
    {
        delete[] _matrix;
        _matrix = rhs._matrix;
        _rowsAmount = rhs._rowsAmount;
        _columnsAmount = rhs._columnsAmount;
        rhs._matrix = nullptr;
        rhs._rowsAmount = 0;
        rhs._columnsAmount = 0;
    }

    ~Matrix() { delete[] _matrix; }

    size_t rowsAmount() const { return _rowsAmount; }

    size_t columnsAmount() const { return _columnsAmount; }

    Matrix sumByRows() const
    {
        MeasureExecution me("Matrix::sumByRows() total time in us: ");
        Matrix m(1, _columnsAmount);
        for (size_t j = 0; j < _columnsAmount; ++j) {
            m._matrix[j] = 0;
            for (size_t i = 0; i < _rowsAmount; ++i)
                m._matrix[j] += (*this)(i, j);
        }
        return m;
    }

    Matrix sumByColumns() const
    {
        MeasureExecution me("Matrix::sumByColumns() total time in us: ");
        Matrix m(_rowsAmount, 1);
        for (size_t i = 0; i < _rowsAmount; ++i) {
            m._matrix[i] = 0;
            for (size_t j = 0; j < _columnsAmount; ++j)
                m._matrix[i] += (*this)(i, j);
        }
        return m;
    }

    Matrix transpose() const
    {
        MeasureExecution me("Matrix::transpose() total time in us: ");
        Matrix m(_columnsAmount, _rowsAmount);
        for (size_t i = 0; i < _rowsAmount; ++i)
            for (size_t j = 0; j < _columnsAmount; ++j)
                m(j, i) = (*this)(i, j);
        return m;
    }

    T& operator()(size_t row, size_t column)
    {
        return _matrix[row*_columnsAmount + column];
    }

    T operator()(size_t row, size_t column) const
    {
        return _matrix[row*_columnsAmount + column];
    }

    Matrix& operator+=(const Matrix& rhs)
    {
        if (rhs._rowsAmount != _rowsAmount ||
            rhs._columnsAmount != _columnsAmount)
            throw std::runtime_error("The matrices are not of the same size");

        for (size_t i = 0; i < _rowsAmount*_columnsAmount; ++i)
            _matrix[i] += rhs._matrix[i];

        return *this;
    }

    Matrix operator+(const Matrix& rhs)
    {
        MeasureExecution me("Matrix::operator+() total time in us: ");
        return Matrix(*this) += rhs;
    }

    Matrix& operator-=(const Matrix& rhs)
    {
        if (rhs._rowsAmount != _rowsAmount ||
            rhs._columnsAmount != _columnsAmount)
            throw std::runtime_error("The matrices are not of the same size");

        for (size_t i = 0; i < _rowsAmount*_columnsAmount; ++i)
            _matrix[i] -= rhs._matrix[i];

        return *this;
    }

    Matrix operator-(const Matrix& rhs)
    {
        MeasureExecution me("Matrix::operator-() total time in us: ");
        return Matrix(*this) -= rhs;
    }

    // Naive matrix multiplication
    Matrix operator*(const Matrix& rhs)
    {
        MeasureExecution me("Matrix::operator*() total time in us: ");

        // TODO: Write the error message
        if (_columnsAmount != rhs._rowsAmount) throw std::runtime_error("");

        Matrix m(_rowsAmount, rhs._columnsAmount);
        for (size_t i = 0; i < _rowsAmount; ++i)
            for (size_t j = 0; j < rhs._columnsAmount; ++j) {
                m(i, j) = 0;
                for (size_t k = 0; k < _columnsAmount; ++k)
                    m(i, j) += (*this)(i, k) * rhs(k, j);
            }
        return m;
    }

private:
    void copyElementsFrom(const Matrix& rhs)
    {
        auto elementsAmount = _rowsAmount * _columnsAmount;
        auto end = rhs._matrix + elementsAmount;
        std::copy(rhs._matrix, end, _matrix);
    }

    size_t _rowsAmount{}, _columnsAmount{};
    T* _matrix{ nullptr };
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m)
{
    for (size_t i = 0; i < m.rowsAmount(); ++i) {
        for (size_t j = 0; j < m.columnsAmount(); ++j)
            os << m(i, j) << ' ';
        os << '\n';
    }
    return os;
}


#endif //MATRIX_MATRIX_H
