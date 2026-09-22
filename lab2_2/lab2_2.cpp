#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

using namespace std;

constexpr double MIN = 10.0;
constexpr double MAX = 100.0;
constexpr int MAX_SIZE = 1200;

/*
 * Ручной ввод элементов массива с клавиатуры
 *
 * @param arr указатель на массив, в который записываются значения.
 * @param N количество элементов массива.
 * @return ничего не возвращает.
 */
void manualInput(double* arr, int N) {
    cout << "Введите числа: " << endl;
    for (int i = 0; i < N; i++) {
        cin >> arr[i];
    }
}

/*
 * Заполнение массива случайными значениями в диапазоне [MIN, MAX]
 *
 * @param arr указатель на массив, в который записываются значения.
 * @param N количество элементов массива.
 * @return ничего не возвращает.
 */
void randomGeneration(double* arr, int N) {
    for (int i = 0; i < N; i++) {
        arr[i] = MIN + rand() * (MAX - MIN) / RAND_MAX;
    }
}

/*
 * Вывод элементов массива на экран
 *
 * @param arr указатель на массив для вывода.
 * @param N количество элементов массива.
 * @return ничего не возвращает.
 */
void print(double* arr, int N) {
    for (int i = 0; i < N; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

/*
 * Поиск минимального и максимального элемента массива
 *
 * @param arr указатель на массив для поиска.
 * @param N количество элементов массива.
 * @return ничего не возвращает (выводит результат на экран).
 */
void maxMin(double* arr, int N) {
    double min = MAX;
    double max = MIN;
    int minInd = -1;
    int maxInd = -1;
    for (int i = 0; i < N; i++) {
        if (arr[i] < min) {
            min = arr[i];
            minInd = i;
        }
        if (arr[i] > max) {
            max = arr[i];
            maxInd = i;
        }
    }
    printf("Минимум: %lf (месяц %d)\n", min, minInd + 1);
    printf("Максимум: %lf (месяц %d)\n", max, maxInd + 1);
}

/*
 * Вычисление среднего значения и стандартного отклонения массива
 *
 * @param arr указатель на массив для вычислений.
 * @param N количество элементов массива.
 * @return ничего не возвращает (выводит результат на экран).
 */
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

/*
 * Сортировка массива по возрастанию методом пузырька
 *
 * @param arr указатель на массив для сортировки.
 * @param N количество элементов массива.
 * @return ничего не возвращает (выводит отсортированный массив на экран).
 */
void sorting(double* arr, int N) {
    for (int i = 0; i < N - 1; i++) {
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

/*
 * Демонстрация передачи массива в функцию по значению
 *
 * @param arr указатель на массив для вывода.
 * @param N количество элементов массива.
 * @return ничего не возвращает.
 */
void byValue(double* arr, int N) {
    cout << "Передача по значению" << endl;
    print(arr, N);
}

/*
 * Демонстрация передачи массива в функцию по ссылке
 *
 * @param arr ссылка на указатель массива.
 * @param N ссылка на количество элементов массива.
 * @return ничего не возвращает.
 */
void byReference(double*& arr, int& N) {
    cout << "Передача по ссылке" << endl;
    print(arr, N);
}



int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(0));
    int N;
    cout << "Введите количество месяцев\n";
    while (true) {
        if (!(cin >> N)) {
            cout << "Введите число" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (N <= 0) {
            cout << "Число месяцев не может быть меньше или равно 0" << endl;
            continue;
        }
        if (N > MAX_SIZE) {
            cout << "Число слишком большое, число месяцев не должно превышать " << MAX_SIZE << endl;
            continue;
        }
        break;
    }
    double* arr = new double[N];
    int mode = 0;
    cout << "Введите 1 для ввода доходов вручную или 2 для генерации случайных чисел" << endl;
    while (true) {
        if (!(cin >> mode) || ((mode != 1) && (mode != 2))) {
            cout << "Введите 1 или 2" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        else if (mode == 1) {
            manualInput(arr, N);
        }
        else {
            randomGeneration(arr, N);
        }
        break;
    }
    print(arr, N);
    int action = -1;
    while (action != 5) {
        cout << "Введите\n" <<
            "1 для поиска месяца с максимальным и минимальным доходом\n" <<
            "2 для вычисления среднегодового дохода и стандартного отклонения\n" <<
            "3 для сортировки массива по возрастанию\n" <<
            "4 для сравнения передачи массива в функцию по значению и по ссылке\n" <<
            "5 для выхода" << endl;
        if (!(cin >> action) || (action < 1) || (action > 5)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        switch (action) {
        case 1:
            maxMin(arr, N);
            break;
        case 2:
            avgStandDev(arr, N);
            break;
        case 3:
            sorting(arr, N);
            break;
        case 4:
            byValue(arr, N);
            byReference(arr, N);
            break;
        default:
            break;
        }
    }
    delete[] arr;
    return 0;
}