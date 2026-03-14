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
#include <filesystem>
#include <zce/zce_convertor.h>

namespace zce {

typedef Matrix<double> matrix_t;

matrix_t mat_identity(int n);

matrix_t mat_add(const matrix_t& A, const matrix_t& B);

matrix_t mat_sub(const matrix_t& A, const matrix_t& B);

matrix_t mat_scale(const matrix_t& A, double s);

matrix_t mat_mul(const matrix_t& A, const matrix_t& B);

double mat_norm1(const matrix_t& A);

matrix_t mat_inv(matrix_t A);

// 矩阵指数：Pade(6) + scaling-squaring（双精度下对 4x4 以内足够稳健）
matrix_t mat_expm(const matrix_t& A);

/**
 * @brief Export zce_dblmat to CSV file
 * @param matrix The matrix to export
 * @param filename The output CSV file path
 * @param delimiter The delimiter character (default: ',')
 * @param precision The precision for floating point numbers (default: 6)
 * @param include_headers Whether to include row/column headers (default: false)
 * @return true if successful, false otherwise
 */
bool export_matrix_to_csv(const matrix_t& matrix, const std::string& filename, char delimiter = ',',
                          int precision = 6, bool include_headers = false);
/**
 * @brief Convert zce_dblmat to CSV string
 * @param matrix The matrix to convert
 * @param delimiter The delimiter character (default: ',')
 * @param precision The precision for floating point numbers (default: 6)
 * @param include_headers Whether to include row/column headers (default: false)
 * @return CSV string representation of the matrix
 */
std::string matrix_to_csv_string(const matrix_t& matrix, char delimiter = ',', int precision = 6,
                                 bool include_headers = false);

/**
 * @brief Import CSV file to zce_dblmat
 * @param filename The input CSV file path
 * @param delimiter The delimiter character (default: ',')
 * @param skip_headers Whether to skip the first row as headers (default: false)
 * @return The imported matrix, empty matrix if failed
 */
zce_dblmat import_matrix_from_csv(const std::string& filename, char delimiter = ',',
                                  bool skip_headers = false);

// 连续到离散：ZOH
void c2d_zoh(const zce::matrix_t& A, const zce::matrix_t& B, double Ts, zce::matrix_t& Ad,
             zce::matrix_t& Bd);

// 计算 ZOH 离散单位阶跃响应（含延迟，按最接近的整采样数处理）
void zoh_step_response(const std::vector<double>& num, const std::vector<double>& den, double Ts,
                       double delay_sec, int N, std::vector<double>& t_out,
                       std::vector<double>& y_out);

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

}  // namespace zce
