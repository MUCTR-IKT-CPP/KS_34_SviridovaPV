// lab5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <ctime>
#include <vector>
#include <string>

constexpr int TACTS_MIN = 10;
constexpr int TACTS_MAX = 20;
constexpr double PLAY_PROBABILITY = 0.4;
constexpr double TUNE_PROBABILITY = 0.2;
constexpr double COEFF_MIN = 0.5;
constexpr double COEFF_MAX = 1.5;
constexpr double TUNE_DECR = 0.1;
constexpr double TUNE_NECESSITY = 0.2;
constexpr int TV_MIN = 5;
constexpr int TV_MAX = 8;
constexpr double BASE_SCORE = 9.0;


class MusicalInstrument {

public:
    enum InstrumentType {
        GUITAR,
        DRUMS,
        PIANO
    };

private:
    std::string name;
    int volume;
    double price;
    double tuning;
    InstrumentType type;

public:
    MusicalInstrument(std::string name, int volume, double price, double tuning, InstrumentType type) :
        name(name), volume(volume), price(price), tuning(tuning), type(type) { }

    ~MusicalInstrument() {}

    std::string getName() const {
        return name;
    }
    int getVolume() const {
        return volume;
    }
    double getPrice() const {
        return price;
    }
    double getTuning() const {
        return tuning;
    }
    InstrumentType getType() const {
        return type;
    }
    void setTuning(double t) {
        tuning = t;
    }

    void play() { };
    void tune() {
        tuning = 100.0;
    }
};


/**
 * @brief Перегрузка оператора вывода в поток для объектов MusicalInstrument.
 * Позволяет выводить информацию об инструменте с помощью std::cout << инструмент.
 * Использует публичные геттеры для доступа к приватным полям.
 * @param outStream Поток вывода (например, std::cout).
 * @param inst Ссылка на константный объект инструмента, который нужно вывести.
 * @return Ссылка на тот же поток вывода (для поддержки цепочек вызовов вида cout << a << b).
 */
std::ostream& operator<<(std::ostream& outStream, const MusicalInstrument& inst) {
    outStream << "Инструмент: " << inst.getName() << " Громкость: " << inst.getVolume() << " Цена: " << inst.getPrice() << " Настройка: " << inst.getTuning() << "\n";
    return outStream;
}


class Guitar: public MusicalInstrument {
private:
    int strings;
public:
    Guitar(std::string name, int volume, double price, double tuning, int strings) :
        strings(strings), MusicalInstrument(name, volume, price, tuning, GUITAR) { }
    void play() { std::cout << "Гитара играет. "; }
};


class Drums : public MusicalInstrument {
private:
    double size;
public:
    Drums(std::string name, int volume, double price, double tuning, double size) :
        size(size), MusicalInstrument(name, volume, price, tuning, DRUMS) { }
    void play() { std::cout << "Барабаны играют. "; }
};


class Piano : public MusicalInstrument {
private:
    int keys;
public:
    Piano(std::string name, int volume, double price, double tuning, int keys) :
        keys(keys), MusicalInstrument(name, volume, price, tuning, PIANO) { }
    void play() { std::cout << "Пианино играет. "; }
};

/**
 * @brief Генерирует случайное целое число в заданном диапазоне [min, max].
 * @param min Минимальное значение (включительно).
 * @param max Максимальное значение (включительно).
 * @return Случайное целое число.
 */
int randInt(int min, int max) { return min + rand() % (max - min + 1); }


/**
 * @brief Генерирует случайное вещественное число в заданном диапазоне [min, max].
 * @param min Минимальное значение.
 * @param max Максимальное значение.
 * @return Случайное число с плавающей точкой.
 */
double randDouble(double min, double max) { return min + rand() / (double)RAND_MAX * (max - min); }


class Conductor {
private:
    int targetVolume = 0;
public:
    Conductor() { }
    int getTargetVolume() const { return targetVolume; }
    void chooseTargetVolume() { targetVolume = randInt(TV_MIN, TV_MAX); }
};


/**
 * @brief Безопасно считывает целое число с консоли с проверкой на пустой ввод, буквы и выход за пределы диапазона.
 * @param min Минимально допустимое значение.
 * @param max Максимально допустимое значение.
 * @return Проверенное целое число.
 */
int getValidatedInt(int min, int max) {
    std::string input;
    while (true) {
        std::getline(std::cin, input);

        if (input.empty()) {
            std::cout << "Введите целое число\n";
            continue;
        }
        
        for (int i = 0; i < input.length(); i++) {
            if (!isdigit(input[i])) {
                std::cout << "Введите целое число без букв и других символов\n";
                goto next_iteration;
            }
        }

        try {
            size_t ind;
            int value = std::stoi(input, &ind);

            if (ind != input.length()) {
                std::cout << "Ввод содержит недопустимые символы\n";
                continue;
            }

            if (value < min || value > max) {
                std::cout << "Число должно быть меньше " << max << " и больше " << min << "\n";
                continue;
            }
            return value;
        }
        catch (std::invalid_argument&) {    
            std::cout << "Вы ввели не число. Введите число\n";
        }
        catch (std::out_of_range&) {
            std::cout << "Число слишком большое\n";
        }
    next_iteration:;
    }
}


/**
 * @brief Безопасно считывает вещественное число с консоли с проверкой на пустой ввод, буквы и отрицательные значения.
 * @param min Минимально допустимое значение (не используется для проверки, оставлено для сигнатуры).
 * @param max Максимально допустимое значение (не используется для проверки, оставлено для сигнатуры).
 * @return Проверенное вещественное число.
 */
double getValidatedDouble(double min, double max) {
    std::string input;

    while (true) {
        std::getline(std::cin, input);

        if (input.empty()) {
            std::cout << "Введите число\n";
            continue;
        }

        for (int i = 0; i < input.length(); i++) {
            if (!isdigit(input[i]) && input[i] != '.' && input[i] != ',') {
                std::cout << "Ошибка: нужно ввести число без букв и других символов\n";
                goto next_iteration;
            }
        }

        try {
            size_t ind;
            double value = std::stod(input, &ind);

            if (ind != input.length()) {
                std::cout << "Число содержит недопустимые символы\n";
                continue;
            }

            //if (value < 0) {
            //    std::cout << "Введите число больше нуля!\n";
            //    continue;
            //}

            return value;
        }
        catch (std::invalid_argument&) {
            std::cout << "Вы ввели не число. Введите число\n";
        }
        catch (std::out_of_range&) {
            std::cout << "Число слишком большое\n";
        }

    next_iteration:;
    }
}


/**
 * @brief Класс, управляющий оркестром музыкальных инструментов.
 * Позволяет добавлять инструменты, проводить репетиции и симулировать концерты.
 */
class Orchestra {

private:
    std::vector<MusicalInstrument*> instruments;

public:
    Orchestra() {}
    ~Orchestra() {
        for (int i = 0; i < instruments.size(); i++) {
            delete instruments[i];
        }
    }

    void clear() {
        for (int i = 0; i < instruments.size(); i++) {
            delete instruments[i];
        }
        instruments.clear();
    }

    /**
     * @brief Добавляет указатель на инструмент в состав оркестра.
     * @param inst Указатель на объект музыкального инструмента.
     */
    void addInstrument(MusicalInstrument* inst) {
        instruments.push_back(inst);
    }

    /**
     * @brief Проводит симуляцию репетиции оркестра.
     * Длительность составляет от 10 до 20 тактов. В каждом такте инструмент
     * играет с вероятностью 40%. Громкость умножается на случайный коэффициент (0.5-1.5).
     * После каждых 5 тактов инструменты могут быть настроены (вероятность 20%).
     * В конце выводит статистику по громкости, сыгранным тактам и времени настройки.
     */
    void rehearsal() {
        if (instruments.empty()) {
            return;
        }

        std::cout << "\nРЕПЕТИЦИЯ\n";

        int tacts = randInt(TACTS_MIN, TACTS_MAX);
        //int tacts = 100;
        std::vector<double> tactsVolume;
        std::vector<int> tactsPerInstrument(instruments.size());
        double sumVolume = 0.0;
        int tuningTime = 0;

        for (int t = 0; t < tacts; t++) {
            double tactVolume = 0.0;
            std::cout << "\nТакт: " << t << "\n";
            int playedCount = 0;

            for (int i = 0; i < instruments.size(); i++) {
                double prob = randDouble(0.0, 1.0);
                if (prob <= PLAY_PROBABILITY) {
                    switch (instruments[i]->getType()) {
                    case MusicalInstrument::GUITAR:
                        static_cast<Guitar*>(instruments[i])->play();
                        break;
                    case MusicalInstrument::DRUMS:
                        static_cast<Drums*>(instruments[i])->play();
                        break;
                    case MusicalInstrument::PIANO:
                        static_cast<Piano*>(instruments[i])->play();
                        break;
                    }

                    playedCount++;

                    double randCoef = randDouble(COEFF_MIN, COEFF_MAX);
                    tactVolume += instruments[i]->getVolume() * randCoef;

                    tactsPerInstrument[i]++;

                    double newTuning = instruments[i]->getTuning() - TUNE_DECR;
                    if (newTuning < 0) newTuning = 0;
                    instruments[i]->setTuning(newTuning);
                }
            }
            if (playedCount == 0) {
                std::cout << "Ни один инструмент не сыграл";
            }

            tactsVolume.push_back(tactVolume);
            sumVolume += tactVolume;
            //std::cout << "Такт " << t + 1 << " Громкость " << tactVolume << "\n";

            if ((t + 1) % 5 == 0) {
                for (int i = 0; i < instruments.size(); i++) {
                    if (randDouble(0.0, 1.0) < TUNE_PROBABILITY) {
                        instruments[i]->setTuning(100.0);
                        tuningTime += 1;
                    }
                }
            }
        }

        printStatistics(tacts, tactsVolume, sumVolume, tactsPerInstrument, tuningTime);
    }


    /**
     * @brief Запрашивает у пользователя параметры для ручного создания инструментов.
     * Позволяет добавить несколько инструментов (Гитара, Барабаны, Пианино).
     * Использует функции валидации для защиты от некорректного ввода.
     */
    void addInstrumentManually() {
        std::cout << "Введите количество инструментов: ";
        int count = getValidatedInt(1, 20);

        for (int i = 0; i < count; i++) {
            std::cout << "Выберите инструмент. 1 - гитара, 2 - барабаны, 3 - пианино\n";
            int type = getValidatedInt(1, 3);

            std::cout << "Введите название: ";
            std::string name;
            std::getline(std::cin, name); 

            std::cout << "Введите громкость: ";
            int volume = getValidatedInt(1, 10);
            std::cout << "Введите цену: ";
            double price = getValidatedDouble(0.0, 100000.0);
            std::cout << "Введите настроенность в процентах: ";
            double tune = getValidatedDouble(1.0, 100.0);

            if (type == 1) {
                std::cout << "Введите количество струн: ";
                int strings = getValidatedInt(1, 12);
                addInstrument(new Guitar(name, volume, price, tune, strings));
            }
            else if (type == 2) {
                std::cout << "Введите размер барабанов: ";
                double size = getValidatedDouble(0.0, 1000.0);
                addInstrument(new Drums(name, volume, price, tune, size));
            }
            else if (type == 3) {
                std::cout << "\nВведите количество клавиш: ";
                int keys = getValidatedInt(1, 108);
                addInstrument(new Piano(name, volume, price, tune, keys));
            }
        }
    }

    /**
     * @brief Выводит информацию обо всех инструментах в оркестре.
     * Использует перегруженный оператор << для форматированного вывода.
     */
    void printOrchestra(){
        std::cout << "\n";
        for (int i = 0; i < instruments.size(); i++) {
            std::cout << *instruments[i];
        }
    }

    /**
     * @brief Выводит статистику по итогам репетиции.
     * @param tacts Количество прошедших тактов.
     * @param tactsVolume Вектор значений громкости для каждого такта.
     * @param sumVolume Суммарная громкость за всю репетицию.
     * @param tactsPerInstrument Вектор с количеством сыгранных тактов каждым инструментом.
     * @param tuningTime Общее количество произведенных настроек.
     */
    void printStatistics(int tacts, std::vector<double> tactsVolume, double sumVolume, std::vector<int> tactsPerInstrument, int tuningTime) {
        std::cout << "\n\nГромкость по тактам\n";
        for (int t = 0; t < tacts; t++) {
            std::cout << "Такт " << t + 1 << " Громкость " << tactsVolume[t] << "\n";
        }

        std::cout << "\nСредняя громкость репетиции " << sumVolume / tacts << "\n";

        std::cout << "\nКоличество игравших тактов для каждого инструмента\n";
        for (int i = 0; i < instruments.size(); i++) {
            std::cout << instruments[i]->getName() << ": " << tactsPerInstrument[i] << "\n";
        }

        std::cout << "\nОбщее время настройки " << tuningTime << "\n";
    }

    /**
     * @brief Симулирует концерт и рассчитывает штрафы за каждый такт.
     * Штраф начисляется за отклонение фактической громкости от целевой громкости дирижера.
     * Если никто не играет, назначается максимальный штраф.
     * @param concertTacts Длительность концерта в тактах.
     * @param penalties Вектор для сохранения штрафов по тактам (передается по ссылке).
     */
    void printPenalties(int concertTacts, std::vector<int>& penalties) {
        std::cout << "\nШтрафы\n";

        Conductor conductor;
        std::vector<double> tactsVolume;
        int sumPenalty = 0;

        for (int t = 0; t < concertTacts; t++) {
            conductor.chooseTargetVolume();
            int targetVolume = conductor.getTargetVolume();
            double tactVolume = 0.0;
            int playedCount = 0;

            for (int i = 0; i < instruments.size(); i++) {
                if (randDouble(0.0, 1.0) < PLAY_PROBABILITY) {
                    double randCoef = randDouble(COEFF_MIN, COEFF_MAX);
                    tactVolume += instruments[i]->getVolume() * randCoef;
                    playedCount++;
                    double newTuning = instruments[i]->getTuning() - TUNE_DECR;
                    if (newTuning < 0) newTuning = 0;
                    instruments[i]->setTuning(newTuning);
                }
            }
            tactsVolume.push_back(tactVolume);

            double deviation = 0.0;
            if (playedCount > 0) {
                deviation = std::abs(targetVolume - tactVolume / playedCount);
                //std::cout << "targetVolume = " << targetVolume;
                //std::cout << " tactVolume = " << tactVolume;
                //std::cout << " playedCount = " << playedCount;
                //std::cout << " tactVolume / playedCount = " << tactVolume / playedCount;

                if (deviation > 4) penalties[t] = 10;
                else if (deviation > 2.5) penalties[t] = 5;
                else if (deviation > 1.5) penalties[t] = 2;
                else penalties[t] = 0;

                sumPenalty += penalties[t];

                std::cout << "Такт: " << t + 1 << ", штраф = " << penalties[t] << "\n";
                //std::cout << "  deviation = " << deviation << "\n";
            }
            else {
                penalties[t] = 20;
                std::cout << "Такт: " << t << ". Не сыграл ни один инструмент" << ". Штраф: " << penalties[t] << "\n";
                sumPenalty += penalties[t];
            }
        }

        std::cout << "\nСуммарный штраф = " << sumPenalty << "\n";
    }

    /**
     * @brief Рассчитывает оценки зрителей на основе полученных штрафов.
     * Оценка зависит от штрафа и случайного "настроения" зала.
     * @param concertTacts Длительность концерта.
     * @param penalties Вектор штрафов (только для чтения).
     * @param audienceScores Вектор для сохранения оценок зрителей (передается по ссылке).
     */
    void printScores(int concertTacts, std::vector<int>& penalties, std::vector<double>& audienceScores) {
        std::cout << "\nОценки\n";

        for (int t = 0; t < concertTacts; t++) {
            double mood = randDouble(-1.0, 1.0);
            audienceScores[t] = BASE_SCORE - (double)penalties[t] * 0.5 + mood;
            if (audienceScores[t] > 10) audienceScores[t] = 10;
            if (audienceScores[t] < 1) audienceScores[t] = 1;
            std::cout << "оценка: " << audienceScores[t] << "\n";
        }
    }

    /**
     * @brief Проводит анализ слаженности оркестра по итогам концерта.
     * Вычисляет среднюю оценку зрителей и выводит её на экран.
     * @param concertTacts Длительность концерта.
     * @param audienceScores Вектор оценок зрителей.
     */
    void analysis(int concertTacts, std::vector<double> audienceScores) {
        double sumScore = 0.0;
        //std::cout << "\n";
        for (int t = 0; t < concertTacts; t++) {
            sumScore += audienceScores[t];
        }
        double avgScore = sumScore / concertTacts;
        std::cout << "\nАнализ: средняя оценка = " << avgScore << "\n\n";
    }

    /**
     * @brief Запускает полную симуляцию концерта.
     * Объединяет расчет штрафов, оценок зрителей и итоговый анализ слаженности.
     */
    void concert() {
        if (instruments.empty()) return; 
        int concertTacts = randInt(TACTS_MIN, TACTS_MAX);
        std::vector<int> penalties(concertTacts, 0);
        std::vector<double> audienceScores(concertTacts, 0);

        std::cout << "\nКОНЦЕРТ\n\n" << "Длительность " << concertTacts << "\n";

        printPenalties(concertTacts, penalties);
        printScores(concertTacts, penalties, audienceScores);
        analysis(concertTacts, audienceScores);        
    }
};


int main()
{
    //std::cout << "Hello World!\n";
    setlocale(LC_ALL, "Russian");
    srand(time(0));

    Orchestra orchestra;

    while (true) {
        std::cout << "1. Использовать инструменты по умолчанию\n";
        std::cout << "2. Ввести инструменты вручную\n";
        std::cout << "3. Выход\n";

        int choice = getValidatedInt(1, 3);

        switch (choice) {
        case 1:
            orchestra.clear();
            orchestra.addInstrument(new Guitar("Guitar", 5, 1500.0, 100.0, 6));
            orchestra.addInstrument(new Drums("Drums", 6, 2000.0, 100.0, 22.0));
            orchestra.addInstrument(new Piano("Piano", 7, 3000.0, 100.0, 88));
            break;
        case 2:
            orchestra.clear();
            orchestra.addInstrumentManually();
            break;
        case 3:
            return 0;
        }

        orchestra.printOrchestra();
        orchestra.rehearsal();
        orchestra.concert();
    }
    
    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
