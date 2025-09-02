#pragma once
// ***************************************************************
//  zce::Matrix_csv   version:  1.0   -  date:  2024/01/01
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project libzce.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#include <zce/zce_types.h>
#include <zce/Matrix.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

namespace zce {

// Helper function to print any matrix regardless of layout
template <typename T, typename Layout>
void print_matrix(const Matrix<T, Layout>& m, const std::string& title) {
    std::cout << "--- " << title << " ---\n";
    if (m.empty()) {
        std::cout << "[Empty Matrix]\n";
        return;
    }
    std::cout << "Dimensions: " << m.rows() << "x" << m.cols() << "\n";
    for (size_t i = 0; i < m.rows(); ++i) {
        for (size_t j = 0; j < m.cols(); ++j) {
            std::cout << m(i, j) << "\t";
        }
        std::cout << "\n";
    }
    std::cout << "Internal vector (_vec) data: ";
    for (const auto& val : m.vec()) {
        std::cout << val << " ";
    }
    std::cout << "\n--------------------------\n\n";
}

/**
 * @brief Export zce_dblmat to CSV file
 * @param matrix The matrix to export
 * @param filename The output CSV file path
 * @param delimiter The delimiter character (default: ',')
 * @param precision The precision for floating point numbers (default: 6)
 * @param include_headers Whether to include row/column headers (default: false)
 * @return true if successful, false otherwise
 */
inline bool export_matrix_to_csv(const zce_dblmat& matrix, const std::string& filename,
                                 char delimiter = ',', int precision = 6,
                                 bool include_headers = false) {
    if (matrix.empty()) {
        return false;
    }

    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    // Set precision for floating point output
    file.precision(precision);
    file << std::fixed;

    // Write headers if requested
    if (include_headers) {
        // Write column headers
        file << "Row/Col";
        for (int j = 0; j < matrix.cols(); ++j) {
            file << delimiter << "Col_" << j;
        }
        file << std::endl;
    }

    // Write matrix data
    for (int i = 0; i < matrix.rows(); ++i) {
        // Write row header if requested
        if (include_headers) {
            file << "Row_" << i << delimiter;
        }

        for (int j = 0; j < matrix.cols(); ++j) {
            file << matrix(i, j);
            if (j < matrix.cols() - 1) {
                file << delimiter;
            }
        }
        file << std::endl;
    }

    file.close();
    return true;
}

/**
 * @brief Convert zce_dblmat to CSV string
 * @param matrix The matrix to convert
 * @param delimiter The delimiter character (default: ',')
 * @param precision The precision for floating point numbers (default: 6)
 * @param include_headers Whether to include row/column headers (default: false)
 * @return CSV string representation of the matrix
 */
inline std::string matrix_to_csv_string(const zce_dblmat& matrix, char delimiter = ',',
                                        int precision = 6, bool include_headers = false) {
    if (matrix.empty()) {
        return "";
    }

    std::ostringstream oss;
    oss.precision(precision);
    oss << std::fixed;

    // Write headers if requested
    if (include_headers) {
        // Write column headers
        oss << "Row/Col";
        for (int j = 0; j < matrix.cols(); ++j) {
            oss << delimiter << "Col_" << j;
        }
        oss << std::endl;
    }

    // Write matrix data
    for (int i = 0; i < matrix.rows(); ++i) {
        // Write row header if requested
        if (include_headers) {
            oss << "Row_" << i << delimiter;
        }

        for (int j = 0; j < matrix.cols(); ++j) {
            oss << matrix(i, j);
            if (j < matrix.cols() - 1) {
                oss << delimiter;
            }
        }
        if (i < matrix.rows() - 1) {
            oss << std::endl;
        }
    }

    return oss.str();
}

/**
 * @brief Import CSV file to zce_dblmat
 * @param filename The input CSV file path
 * @param delimiter The delimiter character (default: ',')
 * @param skip_headers Whether to skip the first row as headers (default: false)
 * @return The imported matrix, empty matrix if failed
 */
inline zce_dblmat import_matrix_from_csv(const std::string& filename, char delimiter = ',',
                                         bool skip_headers = false) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return zce_dblmat();
    }

    std::vector<std::vector<double>> data;
    std::string line;
    int line_count = 0;

    while (std::getline(file, line)) {
        // Skip headers if requested
        if (skip_headers && line_count == 0) {
            line_count++;
            continue;
        }

        std::vector<double> row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, delimiter)) {
            try {
                row.push_back(std::stod(cell));
            } catch (const std::exception&) {
                // Skip invalid cells
                continue;
            }
        }

        if (!row.empty()) {
            data.push_back(row);
        }
        line_count++;
    }

    file.close();

    if (data.empty()) {
        return zce_dblmat();
    }

    // Find the maximum number of columns
    size_t max_cols = 0;
    for (const auto& row : data) {
        max_cols = std::max(max_cols, row.size());
    }

    // Create matrix
    zce_dblmat matrix(static_cast<int>(data.size()), static_cast<int>(max_cols));

    // Fill matrix with data
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < data[i].size(); ++j) {
            matrix(static_cast<int>(i), static_cast<int>(j)) = data[i][j];
        }
        // Fill remaining columns with zeros
        for (size_t j = data[i].size(); j < max_cols; ++j) {
            matrix(static_cast<int>(i), static_cast<int>(j)) = 0.0;
        }
    }

    return matrix;
}

}  // namespace zce
