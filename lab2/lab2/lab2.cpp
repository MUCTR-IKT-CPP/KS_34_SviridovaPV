#include <iostream>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <cmath>

using namespace std;

constexpr double MIN = 10.0;
constexpr double MAX = 100.0;
constexpr int MAX_SIZE = 10000;

void manualInput(double* arr, int N) {
	cout << "Введите числа: " << endl;
	for (int i = 0; i < N; i++) {
		cin >> arr[i];
	}
}

void randomGeneration(double* arr, int N) {
	for (int i = 0; i < N; i++) {
		arr[i] = MIN + rand() * (MAX - MIN) / RAND_MAX;
	}
}

void print(double* arr, int N) {
	for (int i = 0; i < N; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
}

void maxMin(double* arr, int N) {
	double min = MAX;
	double max = MIN;
	for (int i = 0; i < N; i++) {
		if (arr[i] < min) {
			min = arr[i];
		}
		if (arr[i] > max) {
			max = arr[i];
		}
	}
	cout << "Минимум: " << min << endl;
	cout << "Максимум: " << max << endl;
}

void avgStandDev(double* arr, int N) {
	double sum = 0;
	for (int i = 0; i < N; i++) {
		sum += arr[i];
	}
	double avg = sum / N;
	cout << "Среднее значение: " << avg << endl;
	double square_sum = 0;
	for (int i = 0; i < N; i++) {
		square_sum += (arr[i] - avg) * (arr[i] - avg);
	}
	double disp = square_sum / N;
	double stand_dev = sqrt(disp);
	cout << "Стандартное отклонение: " << stand_dev << endl;
}

void sorting(double* arr, int N) {
	for (int i = 0; i = N - 1; i++) {
		bool swapped = false;
		for (int j = 0; j < N - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				swap(arr[j], arr[j + 1]);
				swapped = true;
			}
		}
		if (swapped == false) {
			break;
		}
	}
	cout << "Отсортированный массив:" << endl;
	print(arr, N);
}

void byValue(double* arr, int N) {
	cout << "Передача по значению" << endl;
	print(arr, N);
}

void byReference(double*& arr, int& N) {
	cout << "Передача по ссылке" << endl;
	print(arr, N);
}

int main() {
	srand(time(0));
	SetConsoleOutputCP(65001);
	cout << "Введите количество месяцев\n";
	int N;
	cin >> N;
	double* arr = new double[N];
	int mode = 0;
	while (mode == 0) {
		cout << "Введите 1 для ввода доходов вручную или 2 для генерации случайных чисел\n";
		cin >> mode;
		if (mode == 1) {
			manualInput(arr, N);
		}
		else if (mode == 2) {
			randomGeneration(arr, N);
		}
		else {
			mode = 0;
		}
	}
	print(arr, N);
	int action = 0;
	while (action == 0) {
		cout << "Нажмите 1 для поиска месяца с максимальным и минимальным доходом\n" <<
			"2 для вычисления среднегодового дохода и стандартного отклонения\n" <<
			"3 для сортировки массива по возрастанию\n" <<
			"4 для сравнения передачи массива в функцию по значению и по ссылке\n" <<
			"5 для выхода" << endl;
		cin >> action;
		if (action == 1) {
			maxMin(arr, N);
		}
		else if (action == 2) {
			avgStandDev(arr, N);
		}
		else if (action == 3) {
			sorting(arr, N);
		}
		else if (action == 4) {
			byValue(arr, N);
			byReference(arr, N);
		}
		else if (action == 5) {
			break;
		}
		action = 0;
	}
	delete[] arr;
	return 0;
}