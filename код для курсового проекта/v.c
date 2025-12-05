#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <locale.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_ROOT_ITERATIONS 100

// Основные функции
double f(double x);
double derivative_numerical(double x, double h);
double integral_trapezoidal(double a, double b, int n);
int find_roots_bisection(double y_target, double start, double end, double step, double* roots, int max_roots);

// Вспомогательные функции 
double factorial(int n);
int validate_interval(double* start, double* end);
int validate_positive_integer(int value, const char* parameter_name);
int validate_non_zero_derivative(double x, double h);

// Функции меню
void calculate_value();
void build_table();
void find_extremum();
void find_x_by_y();
void calculate_derivative();
void calculate_integral();

int main() {
    setlocale(LC_ALL, "RUS");
    int choice;

    do {
        printf("\n----ГЛАВНОЕ МЕНЮ----\n");
        printf("1. Значение функции в точке\n");
        printf("2. Таблица значений\n");
        printf("3. Вычисление интеграла\n");
        printf("4. Поиск X по Y\n");
        printf("5. Производная в точке\n");
        printf("6. Поиск экстремумов\n");
        printf("0. Выход\n");
        printf("\nВыберите пункт: ");

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода! Введите число.\n");
            while (getchar() != '\n'); 
            continue;
        }

        switch (choice)
        {
        case 1:
            calculate_value();
            break;
        case 2:
            build_table();
            break;
        case 3:
            calculate_integral();
            break;
        case 4:
            find_x_by_y();
            break;
        case 5:
            calculate_derivative();
            break;
        case 6:
            find_extremum();
            break;
        case 0:
            printf("Выход из программы.\n");
            break;
        default:
            printf("Неверный выбор! Попробуйте снова.\n");
        }
    } while (choice != 0);

    return 0;
}

double f(double x) {
    if (x < -0.5)
    {
        if (fabs(x) < EPSILON) {
            return 2.0;
        }
        return sin(2 * x) / x;
    }
    else if (x >= -0.5 && x < 0) {
        if (fabs(x) < EPSILON) {
            return 0.0;
        }
        return exp(1 / x);
    }
    else {
        double result = 0;

        for (int n = 0; n <= 4; n++)  {
            double factorial_term = factorial(n);
            double numerator = pow(-1, n) * pow(x, 2 * n);
            double denominator = sqrt(factorial_term + 1); // +1 чтобы избежать деления на 0
            result += numerator / denominator;
        }
        return result;
    }
}

//Вычисление факториала числа
double factorial(int n) {
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

//Численное вычисление производной функции в точке
double derivative_numerical(double x, double h) {
    //центральнуf разностную схему для большей точности
    return (f(x + h) - f(x - h)) / (2 * h);
}

//Вычисление определенного интеграла методом трапеций
double integral_trapezoidal(double a, double b, int n) {
    if (n <= 0) return 0.0;

    double h = (b - a) / n;
    double sum = 0.5 * (f(a) + f(b));

    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        sum += f(x);
    }

    return sum * h;
}

//Поиск корней уравнения f(x) = y_target методом бисекции
int find_roots_bisection(double y_target, double start, double end, double step, double* roots, int max_roots) {
    int root_count = 0;
    double current = start;

    while (current < end && root_count < max_roots) {
        double next = current + step;
        if (next > end) next = end;

        double f_current = f(current) - y_target;
        double f_next = f(next) - y_target;

        if (f_current * f_next <= 0) {
            double a = current;
            double b = next;

            // Метод бисекции
            for (int i = 0; i < MAX_ROOT_ITERATIONS; i++) {
                double mid = (a + b) / 2.0;
                double f_mid = f(mid) - y_target;

                if (fabs(f_mid) < EPSILON || (b - a) < EPSILON) {
                    roots[root_count++] = mid;
                    break;
                }

                if (f_current * f_mid <= 0) {
                    b = mid;
                    f_next = f_mid;
                }
                else {
                    a = mid;
                    f_current = f_mid;
                }
            }
        }

        current = next;
    }

    return root_count;
}

// Проверка и коррекция интервала
int validate_interval(double* start, double* end) {
    if (*start > *end) {
        // Меняем местами, если начало больше конца
        double temp = *start;
        *start = *end;
        *end = temp;
        printf("Интервал автоматически скорректирован: [%.2f, %.2f]\n", *start, *end);
    }

    if (fabs(*end - *start) < EPSILON) {
        printf("Ошибка: интервал слишком мал!\n");
        return 0;
    }

    return 1;
}

// Проверка положительного целого числа
int validate_positive_integer(int value, const char* parameter_name) {
    if (value <= 0) {
        printf("Ошибка: %s должно быть положительным числом!\n", parameter_name);
        return 0;
    }
    return 1;
}

// Проверка, что шаг для производной не нулевой
int validate_non_zero_derivative(double x, double h) {
    if (fabs(h) < EPSILON) {
        printf("Ошибка: шаг h слишком мал для вычисления производной!\n");
        return 0;
    }

    // Проверка, что точка ± h не попадает в особые точки
    if (fabs(x + h) < EPSILON || fabs(x - h) < EPSILON) {
        h = 1e-6; // Автоматическая корректировка
        printf("Шаг автоматически скорректирован до %.1e\n", h);
    }

    return 1;
}

// Вычисление значения функции в точке (пункт меню 1)
void calculate_value() {
    printf("\n----Вычисление значения функции в точке----\n");
    double x;
    printf("Введите значение x: ");

    if (scanf("%lf", &x) != 1) {
        printf("Ошибка ввода! Введите число.\n");
        while (getchar() != '\n');
        return;
    }

    double result = f(x);
    printf("f(%.6f) = %.8f\n", x, result);
}

// Построение таблицы значений функции (пункт меню 2)
void build_table() {
    printf("\n----Таблица значений функции----\n");
    double start, end;
    int steps;

    printf("Введите начало интервала: ");
    scanf("%lf", &start);
    printf("Введите конец интервала: ");
    scanf("%lf", &end);

    if (!validate_interval(&start, &end)) return;

    printf("Введите количество шагов: ");
    scanf("%d", &steps);

    if (!validate_positive_integer(steps, "количество шагов")) return;

    double step = (end - start) / steps;

    printf("\n|     x     |    f(x)    |\n");
    printf("|-----------|------------|\n");

    for (int i = 0; i <= steps; i++) {
        double x = start + i * step;
        double y = f(x);
        printf("| %9.6f | %10.8f |\n", x, y);
    }
}

// Вычисление определенного интеграла (пункт меню 3)
void calculate_integral() {
    printf("\n----Вычисление интеграла методом трапеций----\n");

    double a, b;
    int n;

    printf("Введите начало интервала интегрирования: ");
    scanf("%lf", &a);
    printf("Введите конец интервала интегрирования: ");
    scanf("%lf", &b);

    if (!validate_interval(&a, &b)) return;

    printf("Введите количество разбиений: ");
    scanf("%d", &n);

    if (!validate_positive_integer(n, "количество разбиений")) return;

    double result = integral_trapezoidal(a, b, n);
    printf("Интеграл f(x) dx на интервале [%.2f, %.2f] = %.6e\n", a, b, result);
    printf("(метод трапеций, %d разбиений)\n", n);
}

// Поиск X по заданному Y (пункт меню 4)
void find_x_by_y() {
    printf("\n----Поиск X по заданному Y----\n");

    double y_target;
    printf("Введите значение Y для поиска: ");
    scanf("%lf", &y_target);

    double start, end, step;
    printf("Введите начало интервала поиска: ");
    scanf("%lf", &start);
    printf("Введите конец интервала поиска: ");
    scanf("%lf", &end);

    if (!validate_interval(&start, &end)) return;

    printf("Введите шаг разбиения интервала: ");
    scanf("%lf", &step);

    if (step <= 0) {
        printf("Ошибка: шаг должен быть положительным!\n");
        return;
    }

    double roots[100];
    int max_roots = 100;

    int root_count = find_roots_bisection(y_target, start, end, step, roots, max_roots);

    if (root_count == 0) {
        printf("На интервале [%.2f, %.2f] не найдено x, для которых f(x) = %.4f\n",
            start, end, y_target);
    }
    else {
        printf("Найдено %d решений:\n", root_count);
        for (int i = 0; i < root_count; i++) {
            printf("  x%d = %.8f (f(x) = %.8f)\n",
                i + 1, roots[i], f(roots[i]));
        }
    }
}

// Вычисление производной в точке (пункт меню 5)
void calculate_derivative() {
    printf("\n----Вычисление производной в точке----\n");

    double x, h;
    printf("Введите точку для вычисления производной: ");
    scanf("%lf", &x);
    printf("Введите шаг h (рекомендуется 1e-5): ");
    scanf("%lf", &h);

    if (!validate_non_zero_derivative(x, h)) return;

    double derivative = derivative_numerical(x, h);
    printf("f'(%.6f) = %.8f (шаг h = %.1e)\n", x, derivative, h);
    printf("Значение функции в точке: f(%.6f) = %.8f\n", x, f(x));
}

// Поиск экстремумов функции (пункт меню 6)
void find_extremum() {
    printf("\n----Поиск экстремумов функции----\n");

    double start, end, step;
    printf("Введите начало интервала поиска: ");
    scanf("%lf", &start);
    printf("Введите конец интервала поиска: ");
    scanf("%lf", &end);

    if (!validate_interval(&start, &end)) return;

    printf("Введите шаг поиска: ");
    scanf("%lf", &step);

    if (!validate_positive_integer((int)step, "шаг поиска")) return;

    printf("\nПоиск экстремумов на интервале [%.2f, %.2f] с шагом %.4f\n", start, end, step);

    double min_value = INFINITY;
    double max_value = -INFINITY;
    double min_point = start;
    double max_point = start;
    int extremum_count = 0;

    // Поиск минимумов и максимумов перебором
    for (double x = start; x <= end; x += step) {
        double current_value = f(x);

        if (current_value < min_value) {
            min_value = current_value;
            min_point = x;
        }

        if (current_value > max_value) {
            max_value = current_value;
            max_point = x;
        }

        // Поиск локальных экстремумов через производную
        if (x > start + step && x < end - step) {
            double h = 1e-5;
            double deriv_left = derivative_numerical(x - step, h);
            double deriv_center = derivative_numerical(x, h);
            double deriv_right = derivative_numerical(x + step, h);

            // Проверка смены знака производной
            if (deriv_left * deriv_right < 0 && fabs(deriv_center) < 0.1) {
                printf("  Найден возможный экстремум в x ≈ %.6f\n", x);
                printf("    f(x) = %.8f, f'(x) ≈ %.8f\n", current_value, deriv_center);
                extremum_count++;
            }
        }
    }

    printf("\nГлобальные экстремумы на интервале:\n");
    printf("  Минимум: f(%.6f) = %.8f\n", min_point, min_value);
    printf("  Максимум: f(%.6f) = %.8f\n", max_point, max_value);

    if (extremum_count == 0) {
        printf("Локальные экстремумы не обнаружены.\n");
    }
}