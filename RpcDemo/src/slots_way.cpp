#include <zce/zce_config.h>
#include <vector>
#include <iostream>
#include <math.h>

typedef std::vector<int> int_vector;
typedef std::vector<int_vector> int_matrix;
typedef std::vector<double> dbl_vector;
typedef std::vector<dbl_vector> dbl_matrix;

int NCR(int n, int r)
{
    if (r == 0) return 1;

    /*
     Extra computation saving for large R,
     using property:
     N choose R = N choose (N-R)
    */
    if (r > n / 2)
        r = n - r;

    long res = 1;

    for (int k = 1; k <= r; ++k) {
        res *= n - k + 1;
        res /= k;
    }

    return res;
}

enum item_e {
    T, Q, J,
    A, K,
    FROG,
    BIRD,
    PBIRD,
    GBIRD,
    RMAN,
    GMAN,
    WILD,
    SCATTER
};

static const int _row_num[] = {5, 6, 6, 6, 6, 5};

static int_matrix _peifu_matrix = {
    { 1,  2,  3,  4},
    { 1,  2,  3,  4},
    { 1,  2,  3,  4},
    { 4,  6,  8,  10},
    { 4,  6,  8,  10},
    { 6,  10, 12, 15},
    { 6,  10, 12, 15},
    { 8,  15, 20, 30},
    { 10, 25, 40, 60},
    { 20, 25, 50, 70},
    { 30, 40, 60, 80}
};

static int_matrix _relative_expect_matrix = {
    { 1000, 1000, 1000, 1000, 1000, 1000 },
    { 1000, 1000, 1000, 1000, 1000, 1000 },
    { 1000, 1000, 1000, 1000, 1000, 1000 },
    { 800, 800, 800, 800, 800, 800 },
    { 800, 800, 800, 800, 800, 800 },
    { 700, 700, 700, 700, 700, 700 },
    { 700, 700, 700, 700, 700, 700 },
    { 600, 600, 600, 600, 600, 600 },
    { 400, 400, 400, 400, 400, 400 },
    { 300, 300, 300, 300, 300, 300 },
    { 200, 200, 200, 200, 200, 200 },
    { 100, 100, 100, 100, 100, 100 },
    { 100, 100, 100, 100, 100, 100 }
};

template<typename T>
void print_vec(const std::vector<T>& vec)
{
    for (auto& v : vec) {
        std::cout << v << "\t";
    }
    std::cout << std::endl;
}

template <typename T>
void print_vec(const std::vector<std::vector<T>>& vec)
{
    for (auto& v : vec) {
        print_vec(v);
    }
}

static dbl_matrix _calc_p_matrix(const int_matrix& expect_matrix) 
{
    dbl_matrix p_vec(expect_matrix.size());
    dbl_vector sum_vec(expect_matrix[0].size());
    for (auto& vec : expect_matrix) {
        for (int i = 0; i < vec.size(); ++i) {
            sum_vec[i] += vec[i];
        }
    }
    for (int i = 0; i < expect_matrix.size(); ++i) {
        p_vec[i].resize(expect_matrix[i].size());
        for (int j = 0; j < expect_matrix[i].size(); ++j) {
            p_vec[i][j] = expect_matrix[i][j] / sum_vec[j];
        }
    }
    return p_vec;
}

static int_matrix _calc_expect_matrix(const dbl_matrix& p_matrix,  int basenum)
{
    std::vector<std::vector<int>> expect_vec(p_matrix.size());
    for (int i = 0; i < p_matrix.size(); ++i) {
        expect_vec[i].resize(p_matrix[i].size());
        for (int j = 0; j < p_matrix[i].size(); ++j) {
            expect_vec[i][j] = (int)(p_matrix[i][j] * basenum);
        }
    }
    return expect_vec;
}

static dbl_matrix _merge_wild_p_matrix(const dbl_matrix& p_matrix)
{
    dbl_matrix new_p_vec(p_matrix.size());
    for (int i = 0; i < p_matrix.size(); ++i) {
        new_p_vec[i].resize(p_matrix[i].size());
        for (int j = 0; j < p_matrix[i].size(); ++j) {
            new_p_vec[i][j] = p_matrix[i][j] + ((i < WILD) ? p_matrix[WILD][j] : 0);
        }
    }
    return new_p_vec;
}

static dbl_matrix _notp_matrix(const dbl_matrix& p_matrix)
{
    dbl_matrix new_p_vec(p_matrix.size());
    for (int i = 0; i < p_matrix.size(); ++i) {
        new_p_vec[i].resize(p_matrix[i].size());
        for (int j = 0; j < p_matrix[i].size(); ++j) {
            new_p_vec[i][j] =1 - p_matrix[i][j];
        }
    }
    return new_p_vec;
}

static double _calc_rtp_n(const dbl_vector& p_vec, const dbl_vector& notp_vec, const int_vector& peifu_vec, int pengcnt)
{
    ZCE_ASSERT_RETURN(pengcnt >= 3 && pengcnt <= 6, 0);
    //第一列i个，第二列j个，第三列k个，第四列l个，第五列m个，第六列n个的概率*倍数*赔付
    double rtp = 0;
    for (int i = 1; i <= _row_num[0]; ++i) {
        for (int j = 1; j <= _row_num[1]; ++j) {
            for (int k = 1; k <= _row_num[2]; ++k) {
                if (pengcnt == 3) {
                    rtp += NCR(_row_num[0], i) * pow(p_vec[0], i) * pow(notp_vec[0], _row_num[0] - i) * i
                        * NCR(_row_num[1], j) * pow(p_vec[1], j) * pow(notp_vec[1], _row_num[1] - j) * j
                        * NCR(_row_num[2], k) * pow(p_vec[2], k) * pow(notp_vec[2], _row_num[2] - k) * k
                        * notp_vec[3];
                }
                else {
                    for (int l = 1; l <= _row_num[3]; ++l) {
                        if (pengcnt == 4) {
                            rtp += NCR(_row_num[0], i) * pow(p_vec[0], i) * pow(notp_vec[0], _row_num[0] - i) * i
                                * NCR(_row_num[1], j) * pow(p_vec[1], j) * pow(notp_vec[1], _row_num[1] - j) * j
                                * NCR(_row_num[2], k) * pow(p_vec[2], k) * pow(notp_vec[2], _row_num[2] - k) * k
                                * NCR(_row_num[3], l) * pow(p_vec[3], l) * pow(notp_vec[3], _row_num[3] - l) * l
                                * notp_vec[4];
                        }
                        else {
                            for (int m = 1; m <= _row_num[4]; ++m) {
                                if (pengcnt == 5) {
                                    rtp += NCR(_row_num[0], i) * pow(p_vec[0], i) * pow(notp_vec[0], _row_num[0] - i) * i
                                        * NCR(_row_num[1], j) * pow(p_vec[1], j) * pow(notp_vec[1], _row_num[1] - j) * j
                                        * NCR(_row_num[2], k) * pow(p_vec[2], k) * pow(notp_vec[2], _row_num[2] - k) * k
                                        * NCR(_row_num[3], l) * pow(p_vec[3], l) * pow(notp_vec[3], _row_num[3] - l) * l
                                        * NCR(_row_num[4], m) * pow(p_vec[4], m) * pow(notp_vec[4], _row_num[4] - m) * m
                                        * notp_vec[5];
                                }
                                else {
                                    for (int n = 1; n <= _row_num[5]; ++n) {
                                        rtp += NCR(_row_num[0], i) * pow(p_vec[0], i) * pow(notp_vec[0], _row_num[0] - i) * i
                                            * NCR(_row_num[1], j) * pow(p_vec[1], j) * pow(notp_vec[1], _row_num[1] - j) * j
                                            * NCR(_row_num[2], k) * pow(p_vec[2], k) * pow(notp_vec[2], _row_num[2] - k) * k
                                            * NCR(_row_num[3], l) * pow(p_vec[3], l) * pow(notp_vec[3], _row_num[3] - l) * l
                                            * NCR(_row_num[4], m) * pow(p_vec[4], m) * pow(notp_vec[4], _row_num[4] - m) * m
                                            * NCR(_row_num[5], n) * pow(p_vec[5], n) * pow(notp_vec[5], _row_num[5] - n) * n;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return rtp * peifu_vec[pengcnt - 3];
}


static dbl_vector _calc_rtp_x(const dbl_matrix& p_matrix, const dbl_matrix& notp_matrix)
{
    dbl_vector rtp_vec(WILD);
    for (unsigned i = 0; i < WILD; ++i) {
        for (unsigned j = 3; j <= 6; ++j) {
            rtp_vec[i] += _calc_rtp_n(p_matrix[i], notp_matrix[i], _peifu_matrix[i], j);
        }
    }
    return (rtp_vec);
}

void calc_rtp_matrix(const int_matrix& matrix)
{
    std::cout << "---------expect---------" << std::endl;
    print_vec(matrix);

    dbl_matrix p_matrix = _calc_p_matrix(matrix);
    std::cout << "---------p---------" << std::endl;
    print_vec(p_matrix);

    int_matrix expect_matrix = _calc_expect_matrix(p_matrix, 10000);
    std::cout << "---------expect---------" << std::endl;
    print_vec(expect_matrix);

    dbl_matrix new_p_matrix = _merge_wild_p_matrix(p_matrix);
    std::cout << "---------new_p---------" << std::endl;
    print_vec(new_p_matrix);

    dbl_matrix notp_matrix = _notp_matrix(new_p_matrix);
    std::cout << "---------notp---------" << std::endl;
    print_vec(notp_matrix);

    dbl_vector rtp_vec = _calc_rtp_x(new_p_matrix, notp_matrix);
    std::cout << "---------rtp---------" << std::endl;
    print_vec(rtp_vec);

    std::cout << "---------sum---------" << std::endl;
    std::cout << zce_sum(rtp_vec) << std::endl << std::endl << std::endl;
}

static int_matrix _replace(const int_matrix& in, int ival, int rval)
{
    int_matrix new_matrix(in.size());
    for (int i = 0; i < in.size(); ++i) {
        new_matrix[i].resize(in[i].size());
        for (int j = 0; j < in[i].size(); ++j) {
            new_matrix[i][j] = (in[i][j] == ival) ? rval : in[i][j];
        }
    }
    return new_matrix;
}

void calc_rtp()
{
    //calc_rtp_matrix(_relative_expect_matrix);

    int_matrix i2000 = _replace(_relative_expect_matrix, 1000, 1000);
    calc_rtp_matrix(i2000);

    //int_matrix i800 = _replace(_relative_expect_matrix, 1000, 800);
    //calc_rtp_matrix(i800);

    for (int i = 1; i <= 5; ++i) {
        std::cout << NCR(5, i) << " ";
    }
    std::cout << std::endl;
    for (int i = 1; i <= 6; ++i) {
        std::cout << NCR(6, i) << " ";
    }
    std::cout << std::endl;
}