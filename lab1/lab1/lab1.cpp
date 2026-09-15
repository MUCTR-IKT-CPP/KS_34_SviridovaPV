#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <locale.h>
#include <windows.h>
using namespace std;

constexpr int N = 5;
constexpr int M = 4;

/*
 * Заполнение массива случайными значениями
 *
 * @param knights двумерный массив, в который записываются значения.
 * @param N количество рыцарей (строк).
 * @param M количество характеристик каждого рыцаря (столбцов).
 * @param range_min минимальное значение диапазона (по умолчанию 1).
 * @param range_max максимальное значение диапазона (по умолчанию 100).
 * @return ничего не возвращает.
 */
void fillKnightsRandom(int knights[][M], int range_min = 1, int range_max = 100) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            knights[i][j] = rand() % (range_max - range_min + 1) + range_min;
        }
    }
}

/*
 * Вычисление суммы сил одного рыцаря
 *
 * @param knight_forces массив сил одного рыцаря.
 * @param M количество сил.
 * @return возвращает сумму сил рыцаря.
 */
int calculateKnightSum(const int knight_forces[]) {
    int total = 0;
    for (int j = 0; j < M; j++) {
        total += knight_forces[j];
    }
    return total;
}

/*
 * Поиск самого сильного рыцаря (с максимальной суммой характеристик)
 *
 * @param knights двумерный массив рыцарей.
 * @param N количество рыцарей (строк).
 * @param M количество характеристик каждого рыцаря (столбцов).
 * @return возвращает индекс самого сильного рыцаря.
 */
int findMaxKnight(int knights[][M]) {
    int max_index = 0;
    int max_sum = INT_MIN;
    for (int i = 0; i < N; i++) {
        int current_sum = 0;
        for (int j = 0; j < M; j++) {
            current_sum += knights[i][j];
        }
        if (current_sum > max_sum) {
            max_sum = current_sum;
            max_index = i;
        }
    }

    return max_index;
}

/*
 * Поиск самого слабого рыцаря (с минимальной суммой характеристик)
 *
 * @param knights двумерный массив рыцарей.
 * @param N количество рыцарей (строк).
 * @param M количество характеристик каждого рыцаря (столбцов).
 * @return возвращает индекс самого слабого рыцаря.
 */
int findMinKnight(int knights[][M]) {
    int min_index = 0;
    int min_sum = INT_MAX;
    for (int i = 0; i < N; i++) {
        int current_sum = 0;
        for (int j = 0; j < M; j++) {
            current_sum += knights[i][j];
        }
        if (current_sum < min_sum) {
            min_sum = current_sum;
            min_index = i;
        }
    }
    return min_index;
}

/*
 * Вывод результата - индексов самого сильного и самого слабого рыцарей
 *
 * @param max_index индекс самого сильного рыцаря.
 * @param min_index индекс самого слабого рыцаря.
 * @return ничего не возвращает.
 */
void print(int max_index, int min_index){
    printf("Сильнейший рыцарь #%d\n", max_index + 1);
    printf("Слабейший рыцарь #%d\n", min_index + 1);
}

/*
 * Вывод двумерного массива рыцарей на экран
 *
 * @param knights двумерный массив рыцарей.
 * @param N количество рыцарей (строк).
 * @param M количество характеристик каждого рыцаря (столбцов).
 * @return ничего не возвращает.
 */
void printArrays(int knights[][M]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            printf("%5d", knights[i][j]);
        }
        printf("\n");
    }
}


int main()
{
    srand(time(0));

    SetConsoleOutputCP(65001);
    int knights_forces[N][M];

    fillKnightsRandom(knights_forces, 1, 100);
    printArrays(knights_forces);

    int max_index = findMaxKnight(knights_forces);
    int min_index = findMinKnight(knights_forces);

    print(max_index, min_index);
}