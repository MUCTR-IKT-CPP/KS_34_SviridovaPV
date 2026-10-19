// lab4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <ctime>

constexpr float MIN_TEMPERATURE = -100.0;
constexpr float MAX_TEMPERATURE = 500.0;
constexpr float MIN_PRESSURE = 0.0;
constexpr float MAX_PRESSURE = 200000.0;
constexpr int MAX_SIZE = 2000.0;

struct TemperatureSensor {
    float temperature;
};

struct PressureSensor {
    float pressure;
};

struct MotionSensor {
    int motion;
};

enum class SensorType {
    TEMPERATURE,
    PRESSURE,
    MOTION
};

class Sensor {
private:
    void* sensor_data;
    SensorType type;
    int timestamp;

public:
    Sensor() : sensor_data(nullptr), type(SensorType::TEMPERATURE), timestamp(0) {
    }

    Sensor(SensorType t, int time) : sensor_data(nullptr), type(t), timestamp(time) {
        createData();
    }

    Sensor(const Sensor& other) : sensor_data(nullptr), type(other.type), timestamp(other.timestamp) {
        if (other.sensor_data != nullptr) {
            createData();
            if (type == SensorType::TEMPERATURE) {
                *static_cast<TemperatureSensor*>(sensor_data) = *static_cast<TemperatureSensor*>(other.sensor_data);
            }
            else if (type == SensorType::PRESSURE) {
                *static_cast<PressureSensor*>(sensor_data) = *static_cast<PressureSensor*>(other.sensor_data);
            }
            else if (type == SensorType::MOTION) {
                *static_cast<MotionSensor*>(sensor_data) = *static_cast<MotionSensor*>(other.sensor_data);
            }
        }
    }

    Sensor& operator=(const Sensor& other) {
        if (this == &other) {
            return *this;
        }

        deleteData();
        type = other.type;
        timestamp = other.timestamp;
        this->type;

        if (other.sensor_data != nullptr) {
            createData();

            if (type == SensorType::TEMPERATURE) {
                *static_cast<TemperatureSensor*>(sensor_data) = *static_cast<TemperatureSensor*>(other.sensor_data);
            }
            else if (type == SensorType::PRESSURE) {
                *static_cast<PressureSensor*>(sensor_data) = *static_cast<PressureSensor*>(other.sensor_data);
            }
            else if (type == SensorType::MOTION) {
                *static_cast<MotionSensor*>(sensor_data) = *static_cast<MotionSensor*>(other.sensor_data);
            }
        }
        return *this;
    }

    Sensor(Sensor&& other) noexcept
        : sensor_data(other.sensor_data), type(other.type), timestamp(other.timestamp) {
        other.sensor_data = nullptr;
        other.timestamp = 0;
    }

    Sensor& operator=(Sensor&& other) noexcept {
        if (this == &other)
            return *this;
        deleteData();
        sensor_data = other.sensor_data;
        type = other.type;
        timestamp = other.timestamp;
        other.sensor_data = nullptr;
        other.timestamp = 0;
        return *this;
    }

    ~Sensor() {
        deleteData();
    }

    void createData() {
        if (type == SensorType::TEMPERATURE) {
            sensor_data = new TemperatureSensor;
        }
        if (type == SensorType::PRESSURE) {
            sensor_data = new PressureSensor;
        }
        if (type == SensorType::MOTION) {
            sensor_data = new MotionSensor;
        }
    }

    void deleteData() {
        if (sensor_data == nullptr) {
            return;
        }
        if (type == SensorType::TEMPERATURE) {
            delete static_cast<TemperatureSensor*>(sensor_data);
        }
        else if (type == SensorType::PRESSURE) {
            delete static_cast<PressureSensor*>(sensor_data);
        }
        else if (type == SensorType::MOTION) {
            delete static_cast<MotionSensor*>(sensor_data);
        }
        sensor_data = nullptr;
    }

    void generate() {
        if (sensor_data == nullptr) {
            createData();
        }
        if (type == SensorType::TEMPERATURE) {
            TemperatureSensor* data = static_cast<TemperatureSensor*>(sensor_data);
            data->temperature = MIN_TEMPERATURE + static_cast<double>(rand()) / RAND_MAX * (MAX_TEMPERATURE - MIN_TEMPERATURE);
        }
        else if (type == SensorType::PRESSURE) {
            PressureSensor* data = static_cast<PressureSensor*>(sensor_data);
            data->pressure = MIN_PRESSURE + static_cast<double>(rand()) / RAND_MAX * (MAX_PRESSURE - MIN_PRESSURE);
        }
        else if (type == SensorType::MOTION) {
            MotionSensor* data = static_cast<MotionSensor*>(sensor_data);
            data->motion = rand() % 2;
        }
    }

    void calibrate() {
        if (sensor_data == nullptr) {
            return;
        }
        if (type == SensorType::TEMPERATURE) {
            (static_cast<TemperatureSensor*>(sensor_data))->temperature = 0;
        }
        else if (type == SensorType::PRESSURE) {
            (static_cast<PressureSensor*>(sensor_data))->pressure = 0;
        }
        else if (type == SensorType::MOTION) {
            (static_cast<MotionSensor*>(sensor_data))->motion = 0;
        }
    }

    SensorType getType() {
        return type;
    }

    int getTimestamp() const {
        return timestamp;
    }

    void print() {
        std::cout << "Sensor: (Время: " << timestamp << ")\t";
        if (sensor_data == nullptr) {
            std::cout << "Не найден сенсор с таким временем\n";
            return;
        }
        if (type == SensorType::TEMPERATURE) {
            std::cout << "Temperature: " << (static_cast<TemperatureSensor*>(sensor_data))->temperature << "\n";
        }
        else if (type == SensorType::PRESSURE) {
            std::cout << "Pressure: " << (static_cast<PressureSensor*>(sensor_data))->pressure << "\n";
        }
        else if (type == SensorType::MOTION) {
            std::cout << "Motion: " << (static_cast<MotionSensor*>(sensor_data))->motion << "\n";
        }
    }
};

class SensorNetwork {
private:
    Sensor* sensors;
    int count;

public:
    SensorNetwork() : sensors(nullptr), count(0) {
    }

    SensorNetwork(int n) : sensors(new Sensor[n]), count(n) {
        for (int i = 0; i < count; i++) {
            int randomType = rand() % 3;
            if (randomType == 0)
                sensors[i] = Sensor(SensorType::TEMPERATURE, i);
            else if (randomType == 1)
                sensors[i] = Sensor(SensorType::PRESSURE, i);
            else
                sensors[i] = Sensor(SensorType::MOTION, i);
        }
    }

    SensorNetwork(const SensorNetwork& other)
        : sensors(new Sensor[other.count]), count(other.count) {
        for (int i = 0; i < count; i++)
            sensors[i] = other.sensors[i];
    }

    SensorNetwork& operator=(const SensorNetwork& other) {
        if (this == &other) {
            return *this;
        }
        delete[] sensors;
        count = other.count;
        sensors = new Sensor[count];
        for (int i = 0; i < count; i++) {
            sensors[i] = other.sensors[i];
        }
        return *this;
    }

    SensorNetwork(SensorNetwork&& other) noexcept
        : sensors(other.sensors), count(other.count) {
        other.sensors = nullptr;
        other.count = 0;
    }

    SensorNetwork& operator=(SensorNetwork&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        delete[] sensors;
        sensors = other.sensors;
        count = other.count;
        other.sensors = nullptr;
        other.count = 0;
        return *this;
    }

    ~SensorNetwork() {
        delete[] sensors;
    }

    void generateSensors() {
        for (int i = 0; i < count; i++)
            sensors[i].generate();
    }

    void filter(SensorType type) const {
        std::cout << "\nFiltered sensors:\n";

        for (int i = 0; i < count; i++) {
            if (sensors[i].getType() == type)
                sensors[i].print();
        }
    }

    void calibrate() {
        for (int i = 0; i < count; i++)
            sensors[i].calibrate();
    }

    void print() {
        for (int i = 0; i < count; i++)
            sensors[i].print();
    }

    void saveLastValues() const {
        std::cout << "\nПоследние значения\n";
        for (int i = 0; i < count; i++) {
            sensors[i].print();
        }
    }

    void findSensorByTimestamp(int timestamp) const {
        std::cout << "Поиск сенсора по времени, время = " << timestamp << "\n";
        bool found = false;
        for (int i = 0; i < count; i++) {
            if (sensors[i].getTimestamp() == timestamp) {
                sensors[i].print();
                found = true;
                //break;
            }
        }
        if (!found) {
            std::cout << "Не найден сенсор с таким временем\n";
        }
    }
};

int main()
{
    //std::cout << "Hello World!\n";
    setlocale(LC_ALL, "Russian");
    srand(time(0));

    int N;
    std::cout << "Введите количество сенсоров: ";
    while (true) {
        std::cin >> N;
        if (N <= 0) {
            std::cout << "Число сенсоров не может быть меньше или равно 0\n";
            continue;
        }
        if (N > MAX_SIZE) {
            std::cout << "Число слишком большое, число сенсоров не должно превышать " << MAX_SIZE << "\n";
            continue;
        }
        break;
    }

    SensorNetwork network(N);

    std::cout << "generate\n";
    network.generateSensors();
    network.print();

    int choice = -1;
    while (choice != 0) {
        std::cout << "1 - Фильтрация по типу\n";
        std::cout << "2 - Калибровка (сброс значений)\n";
        std::cout << "3 - Сохранить последние значения (журнал)\n";
        std::cout << "4 - Поиск сенсора по времени\n";
        std::cout << "5 - Сгенерировать новые сенсоры\n";
        std::cout << "0 - Выход\n";

        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "\nFilter temperature\n";
            network.filter(SensorType::TEMPERATURE);

            std::cout << "\nFilter pressure\n";
            network.filter(SensorType::PRESSURE);

            std::cout << "\nFilter motion\n";
            network.filter(SensorType::MOTION);
            break;

        case 2:
            std::cout << "\nCalibration\n\n";
            network.calibrate();
            network.print();
            break;

        case 3:
            network.saveLastValues();
            break;

        case 4:
            int inputTime;
            std::cout << "\nПоиск сенсора по времени. Введите время\n";
            std::cin >> inputTime;
            network.findSensorByTimestamp(inputTime);
            break;

        case 5:
            std::cout << "Введите количество сенсоров: ";
            while (true) {
                std::cin >> N;
                if (N <= 0) {
                    std::cout << "Число сенсоров не может быть меньше или равно 0\n";
                    continue;
                }
                if (N > MAX_SIZE) {
                    std::cout << "Число слишком большое, число сенсоров не должно превышать " << MAX_SIZE << "\n";
                    continue;
                }
                break;
            }
            network = SensorNetwork(N);
            std::cout << "generate\n";
            network.generateSensors();
            network.print();
            break;

        case 0:
            break;

        default:
            std::cout << "\nНеверный ввод\n";
        }
    }

    std::cout << "\nCopy\n";
    SensorNetwork copy = network;
    copy.print();

    std::cout << "\nMove \n";
    SensorNetwork moved = std::move(network);
    moved.print();
    Sensor* s = new Sensor();
    s->getType();
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