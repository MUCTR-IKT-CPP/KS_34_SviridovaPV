// lab3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

constexpr int DAY_MIN = 1;
constexpr int DAY_MAX = 30;
constexpr int MONTH_MIN = 1;
constexpr int MONTH_MAX = 12;
constexpr int YEAR_MIN = 2000;
constexpr int YEAR_MAX = 2025;
constexpr double TRANSACTION_MAX = 100;
constexpr double TRANSACTION_MIN = 0;
constexpr int MAX_SIZE = 100000;
constexpr double USD_TO_RUB = 84.53;
constexpr double EUR_TO_RUB = 96.03;
constexpr double CNY_TO_RUB = 12.62;

std::string currencies[] = { "USD", "EUR", "RUB", "CNY" };
std::string categories[] = { "Food", "Salary", "Transport" };

struct Date {
    int day;
    int month;
    int year;
};

struct FinancialTransaction {
    int transaction_id;
    double amount;
    std::string currency;
    bool type;
    std::string category;
    Date date;
};

/*
 * Заполнение массива транзакций случайными значениями
 *
 * @param arr указатель на массив транзакций, который заполняется
 * @param N количество элементов массива
 * @return ничего не возвращает
 */
void randomArray(FinancialTransaction* arr, int N) {
    for (int i = 0; i < N; i++) {
        arr[i].transaction_id = i + 1;
        arr[i].amount = TRANSACTION_MIN + (double)rand() / RAND_MAX * (TRANSACTION_MAX - TRANSACTION_MIN);
        arr[i].currency = currencies[rand() % (sizeof(currencies) / sizeof(currencies[0]))];
        arr[i].type = rand() % 2;
        arr[i].category = categories[rand() % (sizeof(categories) / sizeof(categories[0]))];
        arr[i].date.day = DAY_MIN + rand() % (DAY_MAX - DAY_MIN);
        arr[i].date.month = MONTH_MIN + rand() % (MONTH_MAX - MONTH_MIN);
        arr[i].date.year = YEAR_MIN + rand() % (YEAR_MAX - YEAR_MIN);
    }
}

/*
 * Вывод всех транзакций массива в консоль
 *
 * @param arr указатель на массив транзакций для вывода
 * @param N количество элементов массива
 * @return ничего не возвращает
 */
void printArr(FinancialTransaction* arr, int N) {
    for (int i = 0; i < N; i++) {
        std::cout << "ID транзакции: " << arr[i].transaction_id << '\n';
        std::cout << "Сумма: " << arr[i].amount << '\n';
        std::cout << "Валюта: " << arr[i].currency << '\n';
        std::cout << "Тип: " << (arr[i].type ? "Доход" : "Расход") << '\n';
        std::cout << "Категория: " << arr[i].category << '\n';
        std::cout << "Дата: "
            << arr[i].date.day << '.'
            << arr[i].date.month << '.'
            << arr[i].date.year << '\n';
        std::cout << "\n";
    }
}

/*
 * Подсчёт и вывод суммарного дохода, расхода и баланса в рублях
 *
 * Все суммы приводятся к рублям по фиксированным курсам валют
 *
 * @param arr указатель на массив транзакций
 * @param N количество элементов массива
 * @return ничего не возвращает
 */
void balance(FinancialTransaction* arr, int N) {
    double income = 0;
    double expense = 0;
    double balance = 0;

    for (int i = 0; i < N; i++) {
        double amountRUB = arr[i].amount;

        if (arr[i].currency == "USD")
            amountRUB *= USD_TO_RUB;
        else if (arr[i].currency == "EUR")
            amountRUB *= EUR_TO_RUB;
        else if (arr[i].currency == "CNY")
            amountRUB *= CNY_TO_RUB;

        if (arr[i].type) {
            income += amountRUB;
            balance += amountRUB;
        }
        else {
            expense += amountRUB;
            balance -= amountRUB;
        }
    }

    std::cout << "Суммарный доход: +" << income << " RUB\n";
    std::cout << "Суммарный расход: -" << expense << " RUB\n";
    std::cout << "Баланс: " << balance << " RUB\n";
}

/*
 * Анализ транзакций по категориям с разделением на доходы и расходы
 *
 * Пользователь выбирает тип (доход или расход), после чего
 * выводится сумма по каждой категории в рублях
 *
 * @param arr указатель на массив транзакций
 * @param N количество элементов массива
 * @return ничего не возвращает
 */
void analyse(FinancialTransaction* arr, int N) {

    int choice = -1;
    std::cout << "1 - Доход\n2 - Расход\n";
    while (choice != 1 && choice != 2) {
        std::cin >> choice;
    }

    bool type = (choice == 1);
    double* totals = new double(N);
    int count = 0;

    for (int i = 0; i < N; i++) {
        double amountRUB = arr[i].amount;
        if (arr[i].currency == "USD")
            amountRUB *= USD_TO_RUB;
        else if (arr[i].currency == "EUR")
            amountRUB *= EUR_TO_RUB;

        int j = 0;
        while (j < count && categories[j] != arr[i].category)
            j++;

        if (j == count) {
            categories[count] = arr[i].category;
            totals[count] = 0;
            count++;
        }
        totals[j] += amountRUB;

        std::cout << (type ? "Доход" : "Расход")
            << " по категории:\n";

        if (count == 0) {
            std::cout << "Транзакций не найдено\n";
            return;
        }

        for (int i = 0; i < count; i++) {
            std::cout << categories[i] << ": "
                << totals[i] << " RUB\n";
        }
    }
}

/*
 * Поиск транзакций по заданному месяцу и году
 *
 * Пользователь вводит месяц и год, после чего выводятся
 * все транзакции, относящиеся к указанному периоду
 *
 * @param arr указатель на массив транзакций
 * @param N количество элементов массива
 * @return ничего не возвращает
 */
void search(FinancialTransaction* arr, int N) {
    int month, year;
    std::cout << "Введите месяц: ";
    std::cin >> month;
    while (month < 1 || month > 12) {
        std::cout << "Недопустимое значение\n";
        std::cin >> month;
    }
    std::cout << "Введите год: ";
    std::cin >> year;
    while (year < YEAR_MIN || year > YEAR_MAX) {
        std::cout << "Недопустимое значение\n";
        std::cin >> year;
    }

    FinancialTransaction* result = new FinancialTransaction[N];
    int count = 0;

    for (int i = 0; i < N; i++) {
        if (arr[i].date.month == month &&
            arr[i].date.year == year) {
            result[count] = arr[i];
            count++;
        }
    }

    if (count == 0) {
        std::cout << "Транзакций не найдено";
    }
    else {
        std::cout << "Дата транзакции "
            << month << "/" << year << ":\n";
        printArr(result, count);
    }

    delete[] result;
}

/*
 * Сортировка массива транзакций пузырьком по дате и сумме
 *
 * Сортировка выполняется по возрастанию даты (год, месяц, день),
 * а при равных датах — по убыванию суммы транзакции
 *
 * @param arr указатель на массив транзакций для сортировки
 * @param N количество элементов массива
 * @return ничего не возвращает
 */
void sorting(FinancialTransaction* arr, int N) {
    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N - i - 1; j++) {
            bool swap = false;

            if (arr[j].date.year > arr[j + 1].date.year)
                swap = true;
            else if (arr[j].date.year == arr[j + 1].date.year &&
                arr[j].date.month > arr[j + 1].date.month)
                swap = true;
            else if (arr[j].date.year == arr[j + 1].date.year &&
                arr[j].date.month == arr[j + 1].date.month &&
                arr[j].date.day > arr[j + 1].date.day)
                swap = true;
            else if (arr[j].date.year == arr[j + 1].date.year &&
                arr[j].date.month == arr[j + 1].date.month &&
                arr[j].date.day == arr[j + 1].date.day &&
                arr[j].amount < arr[j + 1].amount)
                swap = true;

            if (swap) {
                FinancialTransaction temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}


/*
 * Точка входа в программу
 *
 * Запрашивает количество транзакций, генерирует случайный массив
 * и предоставляет пользователю меню для работы с ним:
 * расчёт баланса, анализ по категориям, поиск по периоду,
 * сортировка и выход
 *
 * @return код завершения программы (0 — успешное завершение)
 */
int main()
{
    //std::cout << "Hello World!\n";
    setlocale(LC_ALL, "Russian");
    srand(time(0));

    int N;
    while (true) {
        std::cin >> N;
        if (N <= 0) {
            std::cout << "Число месяцев не может быть меньше или равно 0" << std::endl;
            continue;
        }
        if (N > MAX_SIZE) {
            std::cout << "Число слишком большое, число месяцев не должно превышать " << MAX_SIZE << std::endl;
            continue;
        }
        break;
    }
    FinancialTransaction* arr = new FinancialTransaction[N];

    randomArray(arr, N);
    printArr(arr, N);

    int action = -1;
    while (action != 5) {
        std::cout << "Введите\n" <<
            "1 для расчета баланса\n" <<
            "2 для анализа по категориям\n" <<
            "3 для поиска по периоду\n" <<
            "4 для сортировки\n" <<
            "5 для выхода" << std::endl;
        std::cin >> action;
        switch (action) {
        case 1:
            balance(arr, N);
            break;
        case 2:
            analyse(arr, N);
            break;
        case 3:
            search(arr, N);
            break;
        case 4:
            sorting(arr, N);
            printArr(arr, N);
            break;
        case 5:
            break;
        default:
            continue;
        }
    }

    delete[] arr;
    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий файл", чтобы добавить файлы в проект.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.