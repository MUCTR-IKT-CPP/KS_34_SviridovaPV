#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <locale.h>
#include <windows.h>
using namespace std;

const int N = 5;
const int M = 4;
const int MAX_KNIGHTS = 100;
const int MAX_M = 100;

void fillKnightsRandom(int knights[][MAX_M], int N, int M, int rangeMin = 1, int rangeMax = 100) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            knights[i][j] = rand() % (rangeMax - rangeMin + 1) + rangeMin;
        }
    }
}

int calculateKnightSum(const int knightForces[], int M) {
    int total = 0;
    for (int j = 0; j < M; j++) {
        total += knightForces[j];
    }
    return total;
}

int findMaxKnight(int knights[][MAX_M], int N, int M) {
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

int findMinKnight(int knights[][MAX_M], int N, int M) {
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

void print(int maxIndex, int minIndex){
    printf("Самый сильный %d\n", maxIndex + 1);
    printf("Самый слабый %d", minIndex + 1);
}

void printArrays(int knights[][MAX_M], int N, int M) {
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

    int knightsForces[MAX_KNIGHTS][MAX_M];

    fillKnightsRandom(knightsForces, N, M, 1, 100);
    printArrays(knightsForces, N, M);

    int maxIndex = findMaxKnight(knightsForces, N, M);
    int minIndex = findMinKnight(knightsForces, N, M);

    print(maxIndex, minIndex);
}