#include <iomanip>
#include <iostream>
#include <vector>

// Вывод двумерного массива
void print_2(const std::vector<std::vector<double>> &A) {
    for (size_t i = 0; i < 35; i++) std::cout << "-";
    std::cout << std::endl;
    for (const auto &i : A) {
        for (double j : i)
            std::cout << std::right << std::setw(8) << std::setprecision(4) << j << " ";
        std::cout << std::endl;
    }
    for (size_t i = 0; i < 35; i++) std::cout << "-";
    std::cout << '\n'
              << '\n';
}

void print_simplex(const std::vector<std::vector<double>> &A, const std::vector<std::string> &sv, const std::vector<std::string> &basis) {
    for (size_t i = 0; i < 50; i++) std::cout << "-";
    std::cout << std::endl;
    for (const auto &i : sv)
        std::cout << std::right << std::setw(8) << i << " ";
    std::cout << std::endl;
    size_t k = 1;
    for (const auto &i : A) {
        std::cout << std::right << std::setw(8) << basis[k];
        for (double j : i)
            std::cout << std::right << std::setw(8) << std::setprecision(4) << j << " ";
        std::cout << std::endl;
        k++;
    }
    for (size_t i = 0; i < 50; i++) std::cout << "-";
    std::cout << '\n'
              << '\n';
}

// Вывод одномерного массива
void print_1(const std::vector<double> &A) {
    for (size_t i = 0; i < 35; i++) std::cout << "-";
    std::cout << std::endl;
    for (const auto &i : A)
        std::cout << std::setw(8) << i << " ";
    std::cout << std::endl;
    for (size_t i = 0; i < 35; i++) std::cout << "-";
    std::cout << '\n'
              << '\n';
}

// Изначальное заполнение симплекс-таблицы
void first_table(std::vector<std::vector<double>> &A, std::vector<double> c, std::vector<double> &b, std::vector<std::vector<double>> &simplex, const std::vector<std::string> &sv, const std::vector<std::string> &basis) {
    simplex.reserve(A.size() + 1);
    size_t k = 0;
    for (auto &i : A) {
        i.insert(i.begin(), b[k]);
        simplex.push_back(i);
        k++;
    }
    for (double &i : c) i = -i;
    c.insert(c.begin(), 0);
    simplex.push_back(c);
    std::cout << std::setw(20) << "Simplex" << std::endl;
    print_simplex(simplex, sv, basis);
}

// Разрешающий столбец
size_t column(const std::vector<double> &c) {
    size_t column = 10;// в случае оптимального решения функция возвращает 10
    double s = 0;
    for (size_t i = 0; i < 4; i++)
        if (c.at(i) < 0 && c.at(i) < s) {
            s = c.at(i);
            column = i;
        }
    return column;
}

// Разрешающая строка
size_t string(size_t column_, const std::vector<std::vector<double>> &simplex) {
    size_t string_ = 0;// в случае, когда функция не ограничена на множестве решений вернет -1
    double C = -1;
    for (size_t i = 0; i < 3; i++) {
        if (simplex.at(i).at(0) > 0 && simplex.at(i).at(column_) > 0) {
            if (C == -1) string_ = i;
            if (simplex.at(i).at(0) / simplex.at(i).at(column_) < C)
                string_ = i;
            C = simplex.at(i).at(0) / simplex.at(i).at(column_);
        }
    }
    if (C == -1) return -1;
    std::cout << "Element: " << simplex[string_][column_] << " (in column " << column_ + 1 << " and string: " << string_ + 1 << "). Ratio " << simplex.at(string_).at(0) << "/" << simplex.at(string_).at(column_) << " = " << simplex.at(string_).at(0) / simplex.at(string_).at(column_) << std::endl;
    std::cout << '\n';
    return string_;
}

int main() {
    // Заполнение векторов с данными
    std::vector<std::vector<double>> A = {
            {3, 1, 1},
            {1, 2, 0},
            {0, 0.5, 2}};
    std::vector<double> c = {2, 6, 7};
    std::vector<double> b = {3, 8, 1};

    // Вывод матриц из условия
    std::cout << std::setw(18) << "A" << std::endl;
    print_2(A);
    std::cout << std::setw(18) << "c" << std::endl;
    print_1(c);
    std::cout << std::setw(18) << "b" << std::endl;
    print_1(b);

    // Векторa для хранения базисных и свободных переменных:
    // изначально свободные переменные - x1, x2, x3
    std::vector<std::string> sv = {" ", "b", "x1", "x2", "x3"};
    std::vector<std::string> basis = {"-", "x4", "x5", "x6", "F"};

    // Создание и заполенение симплекс таблицы
    std::vector<std::vector<double>> simplex;
    first_table(A, c, b, simplex, sv, basis);

    size_t k = 1;
    // Проверка оптимальности
    while (column(simplex.at(3)) != 10) {
        std::cout << k << ")" << std::endl;

        // Находим разрешающий столбец, строку и элемент
        size_t column_ = column(simplex.at(3));
        std::cout << "Minimum is " << simplex.at(3).at(column_) << std::endl;
        size_t string_ = string(column_, simplex);
        if (string_ == -1) std::cout << "There is no optimal solution" << std::endl;// проверка на то, имеет ли пример вообще оптимальное решение на данно множестве
        else {
            double element = simplex.at(string_).at(column_);
            // Меняем переменные
            std::swap(sv.at(column_ + 1), basis.at(string_ + 1));
            std::cout << "Swap " << sv.at(column_ + 1) << " <-> " << basis.at(string_ + 1) << std::endl;

            // Изменяем таблицу:
            // - элементы разрешающий строки / разрешающий элемент
            // - элементы разрешающего столбца / противоположный разрешающиму элементу
            // - разрешающий элемент заменяется противоположным самому себе (1/element)
            for (size_t i = 0; i < 4; i++)
                for (size_t j = 0; j < 4; j++)
                    if (i != string_ && j != column_)
                        simplex.at(i).at(j) -= simplex.at(string_).at(j) * simplex.at(i).at(column_) / simplex.at(string_).at(column_);

            for (size_t i = 0; i < 4; i++)
                for (size_t j = 0; j < 4; j++) {
                    if (i == string_ && j != column_) simplex.at(i).at(j) /= element;
                    if (i == string_ && j == column_) simplex.at(i).at(j) = 1 / element;
                    if (i != string_ && j == column_) simplex.at(i).at(j) /= (-1) * element;
                }
            // Вывод полученной симплекс-таблицы
            std::cout << std::setw(20) << "Simplex" << std::endl;
            print_simplex(simplex, sv, basis);

            // Подведение итога оптимизации. Если итоговый ответ - оптимальный, выписывается ответ
            if (column(simplex.at(3)) != 10) std::cout << "This is't the optimal solution" << std::endl;
            else {
                std::cout << "This is the optimal solution" << '\n'
                          << '\n';
                std::cout << "ANSWER: F = " << simplex.at(3).at(0) << " (F = 2x1 + 6x2 + 7x3 -> max). "
                          << sv[2] << ", " << sv[3] << ", " << sv[4] << " = 0, " << basis[1] << " = " << simplex.at(0).at(0) << ", " << basis[2] << " = " << simplex.at(1).at(0) << ", " << basis[3] << " = " << simplex.at(2).at(0);
            }
            std::cout << std::endl;

            k++;
        }
    }

    return 0;
}
