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
#include <zce/zce_matrix.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

namespace zce {

typedef Matrix<double> matrix_t;

static matrix_t mat_identity(int n) {
    matrix_t I(n, n);
    for (int i = 0; i < n; ++i) I(i, i) = 1.0;
    return I;
}

static matrix_t mat_add(const matrix_t& A, const matrix_t& B) {
    if (A.rows() != B.rows() || A.cols() != B.cols())
        throw std::runtime_error("mat_add: shape mismatch");
    matrix_t C(A.rows(), A.cols());
    for (int i = 0; i < A.rows() * A.cols(); ++i) C.data()[i] = A.data()[i] + B.data()[i];
    return C;
}

static matrix_t mat_sub(const matrix_t& A, const matrix_t& B) {
    if (A.rows() != B.rows() || A.cols() != B.cols())
        throw std::runtime_error("mat_sub: shape mismatch");
    matrix_t C(A.rows(), A.cols());
    for (int i = 0; i < A.rows() * A.cols(); ++i) C.data()[i] = A.data()[i] - B.data()[i];
    return C;
}

static matrix_t mat_scale(const matrix_t& A, double s) {
    matrix_t C(A.rows(), A.cols());
    for (int i = 0; i < A.size(); ++i) C.data()[i] = A.data()[i] * s;
    return C;
}

static matrix_t mat_mul(const matrix_t& A, const matrix_t& B) {
    if (A.cols() != B.rows()) throw std::runtime_error("mat_mul: shape mismatch");
    matrix_t C(A.rows(), B.cols());
    for (int i = 0; i < A.rows(); ++i) {
        for (int k = 0; k < A.cols(); ++k) {
            double aik = A(i, k);
            if (aik == 0.0) continue;
            for (int j = 0; j < B.cols(); ++j) {
                C(i, j) += aik * B(k, j);
            }
        }
    }
    return C;
}

static double mat_norm1(const matrix_t& A) {
    // 列绝对值和范数
    double m = 0.0;
    for (int j = 0; j < A.cols(); ++j) {
        double col = 0.0;
        for (int i = 0; i < A.rows(); ++i) col += std::fabs(A(i, j));
        if (col > m) m = col;
    }
    return m;
}

static matrix_t mat_inv(matrix_t A) {
    if (A.rows() != A.cols()) throw std::runtime_error("mat_inv: not square");
    int n = A.rows();
    matrix_t X = mat_identity(n);
    // 高斯-约当消元
    for (int i = 0; i < n; ++i) {
        // 选主元
        int piv = i;
        double best = std::fabs(A(i, i));
        for (int r = i + 1; r < n; ++r) {
            double v = std::fabs(A(r, i));
            if (v > best) {
                best = v;
                piv = r;
            }
        }
        if (best < 1e-18) throw std::runtime_error("mat_inv: singular");
        if (piv != i) {
            for (int j = 0; j < n; ++j) {
                std::swap(A(i, j), A(piv, j));
                std::swap(X(i, j), X(piv, j));
            }
        }
        // 归一化
        double diag = A(i, i);
        double invd = 1.0 / diag;
        for (int j = 0; j < n; ++j) {
            A(i, j) *= invd;
            X(i, j) *= invd;
        }
        // 消元
        for (int r = 0; r < n; ++r) {
            if (r == i) continue;
            double f = A(r, i);
            if (f == 0.0) continue;
            for (int j = 0; j < n; ++j) {
                A(r, j) -= f * A(i, j);
                X(r, j) -= f * X(i, j);
            }
        }
    }
    return X;
}

// 矩阵指数：Pade(6) + scaling-squaring（双精度下对 4x4 以内足够稳健）
static matrix_t mat_expm(const matrix_t& A) {
    if (A.rows() != A.cols()) throw std::runtime_error("mat_expm: not square");
    const int n = A.rows();
    const double theta6 = 3.925724783138660;  // Higham 给出的 m=6 阈值
    double n1 = mat_norm1(A);
    int s = 0;
    if (n1 > 0) {
        s = std::max(0, (int)std::ceil(std::log2(n1 / theta6)));
    }
    matrix_t As = mat_scale(A, std::ldexp(1.0, -s));  // A / 2^s

    // Pade(6) 系数 c_k = 1/k!
    const double c0 = 1.0;
    const double c1 = 1.0;
    const double c2 = 1.0 / 2.0;
    const double c3 = 1.0 / 6.0;
    const double c4 = 1.0 / 24.0;
    const double c5 = 1.0 / 120.0;
    const double c6 = 1.0 / 720.0;

    matrix_t I = mat_identity(n);
    matrix_t A2 = mat_mul(As, As);
    matrix_t A4 = mat_mul(A2, A2);
    matrix_t A6 = mat_mul(A4, A2);

    matrix_t U =
        mat_mul(As, mat_add(mat_add(mat_scale(I, c1), mat_scale(A2, c3)), mat_scale(A4, c5)));
    matrix_t V = mat_add(mat_add(mat_scale(I, c0), mat_scale(A2, c2)),
                         mat_add(mat_scale(A4, c4), mat_scale(A6, c6)));

    matrix_t N = mat_add(V, U);
    matrix_t D = mat_sub(V, U);
    matrix_t F = mat_mul(N, mat_inv(D));

    // squaring s 次
    for (int i = 0; i < s; ++i) {
        F = mat_mul(F, F);
    }
    return F;
}

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
inline bool export_matrix_to_csv(const matrix_t& matrix, const std::string& filename,
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
inline std::string matrix_to_csv_string(const matrix_t& matrix, char delimiter = ',',
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
