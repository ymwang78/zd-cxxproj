#pragma once
// ***************************************************************
//  Matrix   version:  1.0   -  date:  2025/09/01
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project libzce.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>  // For std::memcpy
#include <vector>
#include <utility>      // For std::move
#include <type_traits>  // For std::is_same_v, std::is_trivially_copyable_v
#include <stdexcept>    // For std::out_of_range (optional but good practice)
#include <iostream>     // For demonstration

namespace zce {

/**
 * @brief Represents row-major storage order (C, C++, Python style).
 * Elements of a row are contiguous in memory.
 */
struct row_major_layout {};

/**
 * @brief Represents column-major storage order (Fortran, MATLAB, R style).
 * Elements of a column are contiguous in memory.
 */
struct column_major_layout {};
/**
 * @class Matrix
 * @brief A 2D matrix class with customizable memory layout.
 * @tparam T The type of elements to be stored in the matrix.
 * @tparam Layout The memory layout policy. Defaults to row_major_layout.
 * Use row_major_layout or column_major_layout.
 */
template <typename T, typename Layout = row_major_layout>
class Matrix {
    size_t _rows, _cols;
    std::vector<T> _vec;

    /**
     * @brief Calculates the linear index into the _vec for a given row and column.
     * This is the core of the layout logic, resolved at compile time.
     */
    inline size_t get_index(size_t r, size_t c) const {
        if constexpr (std::is_same_v<Layout, row_major_layout>) {
            // Row-major: index = r * num_cols + c
            return r * _cols + c;
        } else {
            // Column-major: index = c * num_rows + r
            return c * _rows + r;
        }
    }

  public:
    Matrix() : _rows(0), _cols(0) {}
    Matrix(size_t rows, size_t cols) : _rows(rows), _cols(cols), _vec(rows * cols) {}
    Matrix(const Matrix& m) : _rows(m._rows), _cols(m._cols), _vec(m._vec) {}
    Matrix(Matrix&& m) noexcept : _rows(m._rows), _cols(m._cols), _vec(std::move(m._vec)) {
        m._rows = m._cols = 0;
    }
    ~Matrix() {}

    Matrix& operator=(const Matrix& m) {
        if (this != &m) {
            _rows = m._rows;
            _cols = m._cols;
            _vec = m._vec;
        }
        return *this;
    }

    Matrix& operator=(Matrix&& m) noexcept {
        if (this != &m) {
            _rows = m._rows;
            _cols = m._cols;
            _vec = std::move(m._vec);
            m._rows = m._cols = 0;
        }
        return *this;
    }

    inline bool operator==(const Matrix& m) const {
        // Note: two matrices with the same values but different layouts will NOT be equal
        // because their underlying _vec will be different.
        return _rows == m._rows && _cols == m._cols && _vec == m._vec;
    }

    inline T& operator()(size_t r, size_t c) {
        // Add bounds checking for safety if desired
        // if (r >= _rows || c >= _cols) throw std::out_of_range("Matrix access out of bounds");
        return _vec[get_index(r, c)];
    }

    inline const T& operator()(size_t r, size_t c) const {
        // if (r >= _rows || c >= _cols) throw std::out_of_range("Matrix access out of bounds");
        return _vec[get_index(r, c)];
    }

    inline void resize(size_t rows, size_t cols) {
        _vec.resize(rows * cols);
        this->_rows = rows;
        this->_cols = cols;
    }

    inline size_t rows() const { return _rows; }
    inline size_t cols() const { return _cols; }
    inline size_t size() const { return _vec.size(); }
    inline const T* data() const { return _vec.data(); }
    inline T* data() { return _vec.data(); }
    inline const std::vector<T>& vec() const { return _vec; }
    inline bool empty() const { return _rows == 0 || _cols == 0; }

    inline void clear() {
        _rows = 0;
        _cols = 0;
        _vec.clear();
    }

    /**
     * @brief Returns a new, transposed copy of the matrix with the same layout.
     */
    inline Matrix<T, Layout> transpose() const {
        if (empty()) {
            return Matrix<T, Layout>();
        }
        Matrix<T, Layout> transposed_matrix(_cols, _rows);
        for (size_t i = 0; i < _rows; ++i) {
            for (size_t j = 0; j < _cols; ++j) {
                transposed_matrix(j, i) = (*this)(i, j);
            }
        }
        return transposed_matrix;
    }

    /**
     * @brief Add a new row. Efficient for row-major, inefficient for column-major.
     */
    inline bool addRow(const std::vector<T>& values = {}) {
        // If matrix is 0x0, but values are provided, infer column count from values.
        if (_rows == 0 && _cols == 0 && !values.empty()) {
            _cols = values.size();
        }
        // If matrix is defined as 0xN, this is a valid state to add a first row.
        else if (!values.empty() && values.size() != _cols) {
            return false;
        }

        const size_t new_rows = _rows + 1;
        std::vector<T> new_vec(new_rows * _cols);

        if constexpr (std::is_same_v<Layout, row_major_layout>) {
            if (!_vec.empty()) {
                if constexpr (std::is_trivially_copyable_v<T>) {
                    std::memcpy(new_vec.data(), _vec.data(), _vec.size() * sizeof(T));
                } else {
                    std::move(_vec.begin(), _vec.end(), new_vec.begin());
                }
            }
            // Add the new row at the end
            for (size_t j = 0; j < _cols; ++j) {
                new_vec[_rows * _cols + j] = values.empty() ? T() : values[j];
            }
        } else {
            if constexpr (std::is_trivially_copyable_v<T>) {
                for (size_t j = 0; j < _cols; ++j) {
                    if (_rows > 0) {
                        std::memcpy(new_vec.data() + j * new_rows, _vec.data() + j * _rows,
                                    _rows * sizeof(T));
                    }
                    new_vec[j * new_rows + _rows] = values.empty() ? T() : values[j];
                }
            } else {
                for (size_t j = 0; j < _cols; ++j) {
                    if (_rows > 0) {
                        std::move(_vec.begin() + j * _rows, _vec.begin() + (j + 1) * _rows,
                                  new_vec.begin() + j * new_rows);
                    }
                    new_vec[j * new_rows + _rows] = values.empty() ? T() : values[j];
                }
            }
        }

        _vec = std::move(new_vec);
        _rows = new_rows;
        return true;
    }

    /**
     * @brief Add a new row with a single value for all columns. (Optimized to avoid intermediate
     * vector)
     * @param value The value to fill in all columns of the new row.
     * @return true if successful, false if matrix has no columns to determine row size.
     */
    inline bool addRow(const T& value) {
        if (_cols == 0 &&
            _rows == 0) {  // Can't add a row to a 0x0 matrix without knowing column count.
            return false;
        }

        const size_t new_rows = _rows + 1;
        std::vector<T> new_vec(new_rows * _cols);

        if constexpr (std::is_same_v<Layout, row_major_layout>) {
            // 1. Copy old data
            if (!_vec.empty()) {
                if constexpr (std::is_trivially_copyable_v<T>) {
                    std::memcpy(new_vec.data(), _vec.data(), _vec.size() * sizeof(T));
                } else {
                    std::move(_vec.begin(), _vec.end(), new_vec.begin());
                }
            }
            // 2. Fill new row directly
            for (size_t j = 0; j < _cols; ++j) {
                new_vec[_rows * _cols + j] = value;
            }
        } else {
            for (size_t j = 0; j < _cols; ++j) {
                // 1. Copy old column data
                if (_rows > 0) {
                    if constexpr (std::is_trivially_copyable_v<T>) {
                        std::memcpy(new_vec.data() + j * new_rows, _vec.data() + j * _rows,
                                    _rows * sizeof(T));
                    } else {
                        std::move(_vec.begin() + j * _rows, _vec.begin() + (j + 1) * _rows,
                                  new_vec.begin() + j * new_rows);
                    }
                }
                // 2. Add new element at the end of the column
                new_vec[j * new_rows + _rows] = value;
            }
        }

        _vec = std::move(new_vec);
        _rows = new_rows;
        return true;
    }

    /**
     * @brief Add a new column. Efficient for column-major, inefficient for row-major.
     */
    inline bool addColumn(const std::vector<T>& values = {}) {
        // If matrix is 0x0, but values are provided, infer row count from values.
        if (_rows == 0 && _cols == 0 && !values.empty()) {
            _rows = values.size();
        }
        // If matrix is defined as Nx0, this is a valid state to add a first column.
        else if (!values.empty() && values.size() != _rows) {
            return false;
        }

        const size_t new_cols = _cols + 1;
        std::vector<T> new_vec(_rows * new_cols);

        if constexpr (std::is_same_v<Layout, row_major_layout>) {
            if constexpr (std::is_trivially_copyable_v<T>) {
                for (size_t i = 0; i < _rows; ++i) {
                    if (_cols > 0) {
                        std::memcpy(new_vec.data() + i * new_cols, _vec.data() + i * _cols,
                                    _cols * sizeof(T));
                    }
                    new_vec[i * new_cols + _cols] = values.empty() ? T() : values[i];
                }
            } else {
                for (size_t i = 0; i < _rows; ++i) {
                    if (_cols > 0) {
                        std::move(_vec.begin() + i * _cols, _vec.begin() + (i + 1) * _cols,
                                  new_vec.begin() + i * new_cols);
                    }
                    new_vec[i * new_cols + _cols] = values.empty() ? T() : values[i];
                }
            }
        } else {
            if (!_vec.empty()) {
                if constexpr (std::is_trivially_copyable_v<T>) {
                    std::memcpy(new_vec.data(), _vec.data(), _vec.size() * sizeof(T));
                } else {
                    std::move(_vec.begin(), _vec.end(), new_vec.begin());
                }
            }
            // Add the new column at the end
            for (size_t i = 0; i < _rows; ++i) {
                new_vec[_cols * _rows + i] = values.empty() ? T() : values[i];
            }
        }

        _vec = std::move(new_vec);
        _cols = new_cols;
        return true;
    }

    /**
     * @brief Add a new column with a single value for all rows. (Optimized to avoid intermediate
     * vector)
     * @param value The value to fill in all rows of the new column.
     * @return true if successful, false if matrix has no rows to determine column size.
     */
    inline bool addColumn(const T& value) {
        if (_rows == 0 &&
            _cols == 0) {  // Can't add a column to a 0x0 matrix without knowing row count.
            return false;
        }

        const size_t new_cols = _cols + 1;
        std::vector<T> new_vec(_rows * new_cols);

        if constexpr (std::is_same_v<Layout, row_major_layout>) {
            // INEFFICIENT PATH for Row-Major
            for (size_t i = 0; i < _rows; ++i) {
                // 1. Copy old row data
                if (_cols > 0) {
                    if constexpr (std::is_trivially_copyable_v<T>) {
                        std::memcpy(new_vec.data() + i * new_cols, _vec.data() + i * _cols,
                                    _cols * sizeof(T));
                    } else {
                        std::move(_vec.begin() + i * _cols, _vec.begin() + (i + 1) * _cols,
                                  new_vec.begin() + i * new_cols);
                    }
                }
                // 2. Add new element at the end of the row
                new_vec[i * new_cols + _cols] = value;
            }
        } else {
            // EFFICIENT PATH for Column-Major
            // 1. Copy old data
            if (!_vec.empty()) {
                if constexpr (std::is_trivially_copyable_v<T>) {
                    std::memcpy(new_vec.data(), _vec.data(), _vec.size() * sizeof(T));
                } else {
                    std::move(_vec.begin(), _vec.end(), new_vec.begin());
                }
            }
            // 2. Fill new column directly
            for (size_t i = 0; i < _rows; ++i) {
                new_vec[_cols * _rows + i] = value;
            }
        }

        _vec = std::move(new_vec);
        _cols = new_cols;
        return true;
    }
};

}  // namespace zce

typedef zce::Matrix<double> zce_dblmat;