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
 * @param rangeMin минимальное значение диапазона (по умолчанию 1).
 * @param rangeMax максимальное значение диапазона (по умолчанию 100).
 * @return ничего не возвращает.
 */
void fillKnightsRandom(int knights[][M], int rangeMin = 1, int rangeMax = 100) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            knights[i][j] = rand() % (rangeMax - rangeMin + 1) + rangeMin;
        }
    }
}

/*
 * Вычисление суммы сил одного рыцаря
 *
 * @param knightForces массив сил одного рыцаря.
 * @param M количество сил.
 * @return возвращает сумму сил рыцаря.
 */
int calculateKnightSum(const int knightForces[]) {
    int total = 0;
    for (int j = 0; j < M; j++) {
        total += knightForces[j];
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
    int maxIndex = 0;
    int maxSum = INT_MIN;
    for (int i = 0; i < N; i++) {
        int currentSum = 0;
        for (int j = 0; j < M; j++) {
            currentSum += knights[i][j];
        }
        if (currentSum > maxSum) {
            maxSum = currentSum;
            maxIndex = i;
        }
    }

    return maxIndex;
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
    int minIndex = 0;
    int minSum = INT_MAX;
    for (int i = 0; i < N; i++) {
        int currentSum = 0;
        for (int j = 0; j < M; j++) {
            currentSum += knights[i][j];
        }
        if (currentSum < minSum) {
            minSum = currentSum;
            minIndex = i;
        }
    }
    return minIndex;
}

/*
 * Вывод результата - индексов самого сильного и самого слабого рыцарей
 *
 * @param maxIndex индекс самого сильного рыцаря.
 * @param minIndex индекс самого слабого рыцаря.
 * @return ничего не возвращает.
 */
void print(int maxIndex, int minIndex){
    printf("Сильнейший рыцарь #%d\n", maxIndex + 1);
    printf("Слабейший рыцарь #%d\n", minIndex + 1);
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
    SetConsoleCP(65001);
    int knightsForces[N][M];

    fillKnightsRandom(knightsForces, 1, 100);
    printArrays(knightsForces);

    int maxIndex = findMaxKnight(knightsForces);
    int minIndex = findMinKnight(knightsForces);

    print(maxIndex, minIndex);
}