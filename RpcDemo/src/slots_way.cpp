#include <zce/zce_config.h>
#include <zce/zce_random.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <numeric>
typedef std::vector<int> int_vector;
typedef std::vector<int_vector> int_matrix;
typedef std::vector<double> dbl_vector;
typedef std::vector<dbl_vector> dbl_matrix;

static int gcd(int a, int b)
{
    while (b > 0) {
        int tmp = a % b;
        a = b;
        b = tmp;
    }
    return a;
}

int NcR(int n, int r)
{
    // p holds the value of n*(n-1)*(n-2)...,
    // k holds the value of r*(r-1)...
    long long p = 1, k = 1;

    if (n - r < r)
        r = n - r;

    if (r == 0)
        return (int)p;

    while (r) {
        p *= n;
        k *= r;

        long long m = std::gcd(p, k);

        p /= m;
        k /= m;

        n--;
        r--;
    }

    return (int)p;
}

int NCR(int n, int r)
{
    if (r == 0) return 1;

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
    FROG, BIRD,
    PBIRD,
    GBIRD,
    RMAN,
    GMAN,
    WILD,
    SCATTER
};

static double _scatter_rate = 0.02;

static const int_vector _row_num = {5, 6, 6, 6, 6, 5};

static const int_vector _scatter_free = {6, 7, 8, 9, 10};

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
    { 1000, 400, 1000, 400, 1000, 400 }, 
    { 400,  1000, 1000, 400, 400, 1000 },
    { 1000, 400, 1000, 500, 1000, 400 },
    { 1000, 400, 800, 400, 800, 400 },
    { 400, 1000, 400, 800, 400, 800 },
    { 1000, 200, 1000, 300, 1000, 300 },
    { 200, 1000, 400, 1000, 400, 1000 },
    { 600, 300, 200, 600, 600, 300 },
    { 400, 100, 400, 200, 400, 200 },
    { 150, 300, 150, 300, 150, 300 },
    { 100, 80, 100, 80, 100, 80 },
    { 100, 100, 100, 100, 100, 100 },
};

template<typename T>
void print_vec(const std::vector<T>& vec)
{
    for (auto& v : vec) {
        std::cout << std::fixed << std::setprecision(7) << v << "\t";
    }
    std::cout << std::endl;
}

template <typename T>
void print_vec(const std::vector<std::vector<T>>& vec, bool will_sum=true)
{
    std::vector<T> sum_vec(vec[0].size());
    for (auto& v : vec) {
        print_vec(v);
        if (will_sum) {
            for (int i = 0; i < v.size(); ++i) {
                sum_vec[i] += v[i];
            }
        }
    }
    if (will_sum) {
        std::cout << std::endl;
        print_vec(sum_vec);
        std::cout << std::endl;
    }
}

//从期望矩阵计算概率矩阵
static dbl_matrix _calc_p_matrix(const int_matrix& expect_matrix) 
{
    dbl_matrix p_vec(expect_matrix.size());
    dbl_vector sum_vec(expect_matrix[0].size());
    for (auto& vec : expect_matrix) {
        for (int i = 0; i < vec.size(); ++i) {
            sum_vec[i] += vec[i];
        }
    }
    for (int i = 0; i < sum_vec.size(); ++i) {
        sum_vec[i] /= (1 - _scatter_rate);
    }

    for (int i = 0; i < expect_matrix.size(); ++i) {
        p_vec[i].resize(expect_matrix[i].size());
        for (int j = 0; j < expect_matrix[i].size(); ++j) {
            p_vec[i][j] = expect_matrix[i][j] / sum_vec[j];
        }
    }
    return p_vec;
}

//从概率矩阵生成随机数分配矩阵
static dbl_matrix _range_p_matrix(const dbl_matrix& p_matrix)
{
    dbl_matrix new_p_vec(p_matrix.size());
    for (int i = 0; i < p_matrix.size(); ++i) {
        new_p_vec[i].resize(p_matrix[i].size());
        for (int j = 0; j < p_matrix[i].size(); ++j) {
            new_p_vec[i][j] = p_matrix[i][j] + ((i > 0) ? new_p_vec[i - 1][j] : 0);
        }
    }
    return new_p_vec;
}

//从概率矩阵创建构建期望矩阵
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

//合并WILD的概率矩阵
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

//计算不出现的概率矩阵
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

//计算某一个图标各种碰的概率
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

//计算每个图标的碰概率
static dbl_matrix _calc_rtp_m(const dbl_matrix& p_matrix, const dbl_matrix& notp_matrix)
{
    dbl_matrix rtp_m(WILD);
    for (unsigned i = 0; i < WILD; ++i) {
        rtp_m[i].resize(4);
        for (unsigned j = 3; j <= 6; ++j) {
            rtp_m[i][j - 3] = _calc_rtp_n(p_matrix[i], notp_matrix[i], _peifu_matrix[i], j);
        }
    }
    return rtp_m;
}

//计算每个图标的RTP概率
static dbl_vector _sum_rtp_m(const dbl_matrix& m_matrix, const dbl_matrix& notp_matrix)
{
    dbl_vector rtp_vec(WILD);
    for (unsigned i = 0; i < WILD; ++i) {
        rtp_vec[i] = zce_sum(m_matrix[i]);
    }
    return (rtp_vec);
}

//计算scatter N碰的概率倍数
static double _scatter_n(int n)
{
    double v = 0;
    int total = zce_sum(_row_num);
    v = NCR(total, n) * pow(_scatter_rate, n) * pow(1 - _scatter_rate, total - n);
    return v;
}

//计算所有scatter的概率倍数
static double _scatter_x()
{
    double v_ge6 = 1;
    double v = 0;
    for (int i = 0; i < 6; ++i) {
        if (i >= 3)
            v += _scatter_n(i) * _scatter_free[i - 3];
        v_ge6 -= _scatter_n(i);
    }
    v += v_ge6 * _scatter_free[6 - 3];
    return v;
}

static int_matrix _generate_item_matrix(const dbl_matrix& range_p_matrix)
{
    int_matrix item_matrix(6);
    zce_random rnd((int)zce_tick());
    for (int i = 0; i < 6; ++i) {
        item_matrix[i].resize(_row_num[i]);
        for (int j = 0; j < _row_num[i]; ++j) {
            double r = rnd.nextdouble();
            item_matrix[i][j] = (int)range_p_matrix.size();
            for (int k = 0; k < range_p_matrix.size(); ++k) {
                if (r < range_p_matrix[k][j]) {
                    item_matrix[i][j] = k;
                    break;
                }
            }
        }
    }
    return item_matrix;
}

static bool _check_item_matrix(const int_matrix& item_matrix)
{
    //WILD 同时出在第一列和第二列就放弃掉，不知道规则
    if (zce_count<int>(item_matrix[0], WILD) > 0 && zce_count<int>(item_matrix[1], WILD) > 0)
        return false;
    return true;
}

static int _calc_peifu(const int_matrix& item_matrix, int_matrix& stat_matrix)
{
    //统计每一种赔付的次数
    stat_matrix.resize(WILD);
    for (int i = 0; i < WILD; ++i) {
        stat_matrix[i].resize(4);
    }

    int ret = 0;
    for (int i = 0; i < item_matrix[0].size(); ++i) {
        for (int j = 0; j < item_matrix[1].size(); ++j) {
            if (item_matrix[0][i] != WILD && item_matrix[1][j] != WILD && item_matrix[0][i] != item_matrix[1][j])
                continue;
            int v_match = (item_matrix[0][i] != WILD) ? item_matrix[0][i] : item_matrix[1][j];
            int_vector v_multi = { 1, 1, 1, 1, 1, 1 };
            v_multi[0] = zce_count<int>(item_matrix[0], WILD) + zce_count<int>(item_matrix[0], v_match);
            v_multi[1] = zce_count<int>(item_matrix[1], WILD) + zce_count<int>(item_matrix[1], v_match);
            for (int k = 0; k < item_matrix[2].size(); ++k) {
                if (item_matrix[2][k] != WILD && v_match != item_matrix[1][j])
                    continue;
                for (int l = 0; l < item_matrix[3].size(); ++l) {
                    if (item_matrix[3][l] != WILD && v_match != item_matrix[3][l]) {
                        //3 碰
                        ++stat_matrix[i][0];
                        continue;
                    }
                    for (int m = 0; m < item_matrix[4].size(); ++m) {
                        for (int n = 0; n < item_matrix[5].size(); ++n) {
                            int_vector vec = { item_matrix[0][i], item_matrix[1][j], item_matrix[2][k], item_matrix[3][l], item_matrix[4][m], item_matrix[5][n] };
                            //std::sort(vec.begin(), vec.end());
                            //if (vec[0] == vec[1] && vec[1] == vec[2] && vec[2] == vec[3] && vec[3] == vec[4]) {
                            //    ret += _peifu_matrix[vec[0]][5];
                            //}
                            //else if (vec[0] == vec[1] && vec[1] == vec[2] && vec[2] == vec[3]) {
                            //    ret += _peifu_matrix[vec[0]][4];
                            //}
                            //else if (vec[1] == vec[2] && vec[2] == vec[3] && vec[3] == vec[4]) {
                            //    ret += _peifu_matrix[vec[1]][4];
                            //}
                            //else if (vec[0] == vec[1] && vec[1] == vec[2]) {
                            //    ret += _peifu_matrix[vec[0]][3];
                            //}
                            //else if (vec[1] == vec[2] && vec[2] == vec[3]) {
                            //    ret += _peifu_matrix[vec[1]][3];
                            //}
                            //else if (vec[2] == vec[3] && vec[3] == vec[4]) {
                            //    ret += _peifu_matrix[vec[2]][3];
                            //}
                            //else if (vec[0] == vec[1] && vec[2] == vec[3]) {
                            //    ret += _peifu_matrix[vec[0]][2];
                            //}
                            //else if (vec[0] == vec[1] && vec[3] == vec[4]) {
                            //    ret += _peifu_matrix[vec]
                            //}
                        }
                    }
                }
            }
        }
    }
    return ret;
}

void calc_rtp_matrix(const int_matrix& matrix)
{
    std::cout << "---------expect---------" << std::endl;
    print_vec(matrix);

    dbl_matrix p_matrix = _calc_p_matrix(matrix);
    std::cout << "---------p---------" << std::endl;
    print_vec(p_matrix);

    dbl_matrix r_p_matrix = _range_p_matrix(p_matrix);
    std::cout << "---------range_p_matrix---------" << std::endl;
    print_vec(r_p_matrix);

    int_matrix expect_matrix = _calc_expect_matrix(p_matrix, 10000);
    std::cout << "---------expect---------" << std::endl;
    print_vec(expect_matrix);

    dbl_matrix new_p_matrix = _merge_wild_p_matrix(p_matrix);
    std::cout << "---------new_p---------" << std::endl;
    print_vec(new_p_matrix);

    dbl_matrix notp_matrix = _notp_matrix(new_p_matrix);
    std::cout << "---------notp---------" << std::endl;
    print_vec(notp_matrix);

    dbl_matrix rtp_matrix = _calc_rtp_m(new_p_matrix, notp_matrix);
    std::cout << "---------rtp matrix---------" << std::endl;
    print_vec(rtp_matrix);

    dbl_vector rtp_vec = _sum_rtp_m(rtp_matrix, notp_matrix);
    std::cout << "---------rtp---------" << std::endl;
    print_vec(rtp_vec);

    std::cout << "---------sum---------" << std::endl;
    std::cout << zce_sum(rtp_vec) << std::endl << std::endl << std::endl;

    std::cout << "---------scatter_x---------" << std::endl;
    std::cout << _scatter_x() << std::endl << std::endl << std::endl;

    std::cout << "---------final rtp---------" << std::endl;
    std::cout << zce_sum(rtp_vec) / (1 - _scatter_x()) << std::endl << std::endl << std::endl;

    int_matrix item_matrix = _generate_item_matrix(r_p_matrix);
    std::cout << "---------an item matrix---------" << std::endl;
    print_vec(item_matrix, false);
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

    int_matrix i2000 = _replace(_relative_expect_matrix, 1000, 2000);
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
