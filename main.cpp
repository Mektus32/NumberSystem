#include <iostream>
#include <vector>
#include <chrono>
#include <array>
#include <limits>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <boost/algorithm/string.hpp>

std::vector<std::size_t> T;
std::size_t a1, a2, a3, a4 = std::numeric_limits<std::size_t>::max();

bool IsFilledA4() {
    return a4 != std::numeric_limits<std::size_t>::max();
}

std::size_t GetT(std::size_t i) {
    if (T.size() > i) {
        return T[i];
    }

    std::size_t result;
    if (IsFilledA4()) {
        result = a4 * T[i - 1] + a3 * T[i - 2] + a2 * T[i - 3] + a1 * T[i - 4];
    } else {
        result = a3 * T[i - 1] + a2 * T[i - 2] + a1 * T[i - 3];
    }

    T.emplace_back(result);
    return result;
}

void Alignment(std::vector<std::vector<std::size_t>> &coefficients) {
    const auto it = std::max_element(coefficients.begin(), coefficients.end(), [](const auto &lhs, const auto &rhs) {
        return lhs.size() < rhs.size();
    });
    for (auto &coef: coefficients) {
        coef.reserve(it->size());
        coef.insert(coef.begin(), it->size() - coef.size(), 0);
    }
}

bool IsEqual(const std::vector<std::size_t> &lhs, const std::vector<std::size_t> &rhs, std::size_t last) {
    auto tmp1 = std::vector<std::size_t>{lhs.end() - last, lhs.end()};
    auto tmp2 = std::vector<std::size_t>{rhs.end() - last, rhs.end()};
    return tmp1 == tmp2;
}

//void Test(const std::vector<std::vector<std::size_t>>& coefficients, std::size_t last) {
//    std::fstream test("../test.txt", std::ios_base::in);
//
//    std::vector<std::size_t> tmp(16, 0);
//    std::vector<std::vector<std::size_t>> test_data;
//    test_data.emplace_back(tmp);
//    for (std::string line; std::getline(test, line); ) {
//        std::vector<std::string> strs;
//        boost::split(strs,line,boost::is_any_of(" "));
//        test_data.emplace_back();
//        std::transform(strs.begin(), strs.end(), std::back_inserter(*(test_data.end() - 1)), [](const auto& value){
//            return std::stoull(value);
//        });
//    }
//
//    for (std::size_t i = 0; i < test_data.size(); ++i) {
//        if (!IsEqual(test_data[i], coefficients[i], last)) {
//            std::cout << "i: " << i;
//            exit(0);
//        }
//    }
//}

//void BeautifulPrint(std::ostream& out, const std::vector<std::vector<std::size_t>>& coefficients, std::size_t last) {
//    out.clear();
//    for (std::size_t i = 1; i < coefficients.size(); ++i) {
//        out << i << ": ";
//        for (const auto &coef : coefficients[i]) {
//            out << coef;
//        }
//        out << std::endl;
//    }
//}

std::vector<std::vector<std::size_t>> FillCoefficients(std::size_t n) {

    T.reserve(n / 10'000 > 20 ? n / 10'000 : 20); // reserve 20 or more T
    T.emplace_back(1);
    T.emplace_back(a1 + 1);
    T.emplace_back(a1 * a2 + a1 + a2 + 1);
    if (IsFilledA4()) {
        T.emplace_back(1 + a1 + a2 + a1 * a2 + a3 + a1 * a3);
    }

//    const auto start = std::chrono::system_clock::now();

    std::vector<std::vector<std::size_t>> coefficients(n + 1);
    for (std::size_t i = 1; i <= n; ++i) {
        std::size_t tmp_n = i;

        std::size_t k{0};
        while (true) {
            std::size_t Tk = GetT(k);
            if (Tk > tmp_n) {
                break;
            }
            ++k;
        }

        std::vector<std::size_t> coef;
        coef.reserve(k);
        for (std::size_t j = 0; k > 0; --k, ++j) {
            std::size_t Tk = GetT(k);
            if (Tk <= tmp_n) {
                continue;
            }
            std::size_t Tkminus1 = GetT(k - 1);
            if (std::size_t tk = tmp_n / Tkminus1; tk >= 1) {
                coef.emplace_back(tk);
                tmp_n -= tk * Tkminus1;
            } else {
                coef.emplace_back(0);
            }
        }
        coefficients[i] = std::move(coef);
    }

//        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() << std::endl;

    return coefficients;
}

std::string VectorToString(const std::vector<std::size_t> &vec, std::size_t last) {
    std::string str;
    for (auto iter = vec.end() - last; iter != vec.end(); ++iter) {
        str += std::to_string(*iter);
    }
    return str;
}

std::unordered_set<std::size_t>
GetDifferenceBetweenNumbersWithEqualLastCoefficients(const std::vector<std::vector<std::size_t>> &coefficients,
                                                     std::size_t last) {
    std::unordered_set<std::size_t> result;
    result.reserve(30);

    std::unordered_map<std::string, std::vector<std::size_t>> remains;
    remains.reserve(100);

//    const auto start = std::chrono::system_clock::now();

    for (std::size_t i = 1; i < coefficients.size(); ++i) {
        remains[VectorToString(coefficients[i], last)].emplace_back(i);
    }

    for (const auto &remain: remains) {
        for (std::size_t i = 0; i + 1 < remain.second.size(); ++i) {
            result.insert(remain.second[i + 1] - remain.second[i]);
        }
    }

//    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();

    return result;
}

std::tuple<std::size_t, std::size_t> ParseProgramArgs(int argc, char **argv) {
    std::size_t n{0}, last{0};

    for (int i = 1; i < argc; ++i) {
        std::vector<std::string> strs;
        boost::split(strs, argv[i], boost::is_any_of("="));
        if (strs.size() != 2) {
            std::cout << "Wrong input";
            exit(-1);
        }
        if (strs[0] == "a1") {
            a1 = std::stoull(strs[1]);
        } else if (strs[0] == "a2") {
            a2 = std::stoull(strs[1]);
        } else if (strs[0] == "a3") {
            a3 = std::stoull(strs[1]);
        } else if (strs[0] == "a4") {
            a4 = std::stoull(strs[1]);
        } else if (strs[0] == "n") {
            n = std::stoull(strs[1]);
        } else if (strs[0] == "last") {
            last = std::stoull(strs[1]);
        }
    }

    if (!n || !last) {
        std::cout << "Wrong input";
        exit(-1);
    }

    return {n, last};
}

int main(int argc, char **argv) {
    std::fstream file{"output.txt", std::ios_base::out};

    std::size_t n, last;
    std::tie(n, last) = ParseProgramArgs(argc, argv);

    if (IsFilledA4()) {
        if (!(a1 <= a2 && a2 <= a3 && a3 <= a4)) {
            std::cout << "Wrong input";
            return -1;
        }
        file << "a1:" << a1 << ", a2:" << a2 << ", a3:" << a3 << ", a4:" << a4 << ", n:" << n << ", last:" << last
             << std::endl;
    } else {
        if (!(a1 <= a2 && a2 <= a3)) {
            std::cout << "Wrong input";
            return -1;
        }
        file << "a1:" << a1 << ", a2:" << a2 << ", a3:" << a3 << ", n:" << n << ", last:" << last << std::endl;
    }

    auto coefficients = FillCoefficients(n);

    Alignment(coefficients);

//    Test(coefficients, last);

//    BeautifulPrint(file, coefficients, last);

    const auto differences = GetDifferenceBetweenNumbersWithEqualLastCoefficients(coefficients, last);

    std::unordered_set<std::string> result;
    result.reserve(differences.size());
    std::transform(differences.begin(), differences.end(), std::insert_iterator(result, result.begin()),
                   [&coefficients, &last](const auto &value) {
                       return VectorToString(coefficients[value], last);
                   });

    for (const auto &val: result) {
        file << val << std::endl;
    }

    return 0;
}
