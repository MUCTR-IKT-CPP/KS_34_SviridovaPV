// lab6.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <ctime>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <string>
#include <stdexcept>

constexpr int TURNS = 5;

class Character;

class ISkills {
public:
    virtual ~ISkills() = default;
    virtual void useSpecialAbility(Character& target) = 0;
};


int randInt(int min, int max) {
    return min + rand() % (max - min + 1);
}
double randDouble(double min, double max) {
    return min + rand() / (double)RAND_MAX * (max - min);
}


class Character {
protected:
    int id;
    std::string name;
    int health;
    int maxHealth;
    int attackPower;
    int defense;
    virtual void printHeader() const = 0;
    virtual void printExtra() const = 0;

public:
    void getStats() const {
        printHeader();
        std::cout << " | HP: " << std::setw(3) << health
            << " | ATK: " << std::setw(3) << attackPower
            << " | DEF: " << std::setw(3) << defense;
        printExtra();
        std::cout << '\n';
    }

    Character(int id, std::string name, int hp, int atk, int def) :
        id{ id }, 
        name{ std::move(name) }, 
        health{ hp },
        maxHealth{ hp },
        attackPower{ atk },
        defense{ def } { }

    virtual ~Character() = default;

    virtual void attack(Character& target)  = 0;
    virtual void defend() = 0;

    int getId() const { return id; }
    const std::string& getName() const { return name; }
    bool isAlive() const { return health > 0; }
    int getHealth() const { return health; }

    void takeDamage(int damage) {
        const int actual{ std::max(0, damage - defense) };
        health = std::max(0, health - actual);
        std::cout << name << " получает " << actual
            << " урона (HP: " << health << "/" << maxHealth << ")\n";
    }

    void heal(int amount) {
        health = std::min(maxHealth, health + amount);
        std::cout << name << " восстанавливает " << amount
            << " HP (HP: " << health << "/" << maxHealth << ")\n";
    }

protected:
    int rawDamage() const { return attackPower; }
    int rollDamage() const {
        return std::max(1, rawDamage() + randInt(-2, 2));
    }
};


class Warrior final : public Character, public ISkills {
    std::string armorType;

public:
    Warrior(int id, std::string name, int hp, int atk, int def, std::string armor) :
        Character{ id, std::move(name), hp, atk, def },
        armorType{ std::move(armor) } { }

    void attack(Character& target) override {
        int dmg{ rollDamage() };
        if (randDouble(0.0, 1.0) < 0.20) {
            dmg *= 2;
            std::cout << "Warrior " << name << " критический удар по "
                << target.getName() << "\n";
        }
        else {
            std::cout << "Warrior " << name << " удар мечом " << target.getName() << '\n';
        }
        target.takeDamage(dmg);
    }

    void defend() override {
        defense += 5;
        std::cout << "Warrior " << name << " защищается (DEF: " << defense << ")\n";
    }

    void useSpecialAbility(Character& target) override {
        std::cout << "[Warrior-Ability] " << name
            << " бьёт дважды\n";
        target.takeDamage(rawDamage());
        target.takeDamage(rawDamage());
    }

protected:
    void printHeader() const override {
        std::cout << std::left << std::setw(8) << "Warrior"
            << "| id=" << std::setw(3) << id
            << "| " << std::setw(10) << name;
    }
    void printExtra() const override {
        std::cout << " | " << armorType;
    }
};


class Mage final : public Character, public ISkills {
    int manaPoints;
public:
    Mage(int id, std::string name, int hp, int atk, int def, int mana) :
        Character{ id, std::move(name), hp, atk, def },
        manaPoints{ mana } { }

    void attack(Character& target) override {
        if (manaPoints < 10) {
            std::cout << "Mage " << name
                << " без маны слабый удар посохом\n";
            target.takeDamage(rollDamage() / 2);
            return;
        }
        std::cout << "Mage " << name << " бросает огненный шар в "
            << target.getName() << '\n';
        target.takeDamage(rollDamage() * 2);
        manaPoints -= 10;
    }

    void defend() override {
        if (manaPoints >= 5) {
            manaPoints -= 5;
            defense += 3;
            std::cout << "Mage " << name << " ставит защиту (DEF: "
                << defense << ", mana: " << manaPoints << ")\n";
        }
        else {
            std::cout << "Mage " << name
                << " без маны —> защита не удалась.\n";
        }
    }

    void useSpecialAbility(Character& target) override {
        if (manaPoints < 20) {
            std::cout << "[Mage-Ability] " << name
                << " —> недостаточно маны для исцеления\n";
            return;
        }
        manaPoints -= 20;
        std::cout << "[Mage-Ability] " << name << " исцеляет "
            << target.getName() << '\n';
        target.heal(25);
    }

protected:
    void printHeader() const override {
        std::cout << std::left << std::setw(8) << "Mage"
            << "| id=" << std::setw(3) << id
            << "| " << std::setw(10) << name;
    }
    void printExtra() const override {
        std::cout << " | mana: " << manaPoints;
    }
};


class Archer final : public Character, public ISkills {
    int arrowCount;
public:
    Archer(int id, std::string name, int hp, int atk, int def, int arrows) :
        Character{ id, std::move(name), hp, atk, def },
        arrowCount{ arrows } { }

    void attack(Character& target) override {
        if (arrowCount <= 0) {
            std::cout << "Archer " << name
                << " без стрел переходит в ближний бой.\n";
            target.takeDamage(rawDamage() / 3);
            return;
        }
        std::cout << "Archer " << name << " стреляет в " << target.getName() << "\n";
        target.takeDamage(rawDamage());
        arrowCount--;
    }

    void defend() override {
        defense += 4;
        std::cout << "Archer " << name << " уклоняется. Защита: " << defense << '\n';
    }

    void useSpecialAbility(Character& target) override {
        std::cout << "[Archer-Ability] " << name
            << " выпускает дождь стрел\n";
        const int shots{ randInt(2, 4) };
        for (int i = 0; i < shots; ++i)
            target.takeDamage(rollDamage() / 2);
    }

protected:
    void printHeader() const override {
        std::cout << std::left << std::setw(8) << "Archer"
            << "| id=" << std::setw(3) << id
            << "| " << std::setw(10) << name;
    }
    void printExtra() const override {
        std::cout << " | arrows: " << arrowCount;
    }
};


class Party {
    std::string name;
    std::vector<std::unique_ptr<Character>> members;

public:
    explicit Party(std::string name) : name{ std::move(name) } {}

    void addMember(Character* c) {
        if (!c) return;
        members.emplace_back(c);
    }

    void removeMember(int id) {
        const auto oldSize{ members.size() };
        members.erase(
            std::remove_if(members.begin(), members.end(),
                [id](const std::unique_ptr<Character>& p) {
                    return p->getId() == id;
                }),
            members.end());
        if (members.size() == oldSize)
            std::cout << "[!] Персонаж с id=" << id << " не найден\n";
    }

    void partyAttack(Party& enemy) {
        if (enemy.aliveCount() == 0) return;
        std::cout << "\n--- " << name << " атакует " << enemy.getName() << " ---\n";
        for (auto& member : members) {
            if (!member->isAlive()) continue;
            Character& target = enemy.randomAliveTarget();
            member->attack(target);
            if (!enemy.anyAlive()) break;
        }
    }

    void partyUseSkills(Party& enemy) {
        std::cout << "\n--- " << name << " применяет умения ---\n";
        for (auto& member : members) {
            if (!member->isAlive()) continue;

            auto* skills{ dynamic_cast<ISkills*>(member.get()) };
            if (!skills) continue;

            if (dynamic_cast<Mage*>(member.get()) && aliveCount() > 1) {
                Character& ally = randomAllyExcept(member->getId());
                if (ally.isAlive())
                    skills->useSpecialAbility(ally);
            }
            else if (enemy.anyAlive()) {
                skills->useSpecialAbility(enemy.randomAliveTarget());
            }
        }
    }

    Character& randomAllyExcept(int id) {
        std::vector<Character*> alive;
        for (auto& p : members)
            if (p->isAlive() && p->getId() != id) alive.push_back(p.get());

        if (alive.empty()) return *members.front();
        const int idx{ randInt(0, static_cast<int>(alive.size()) - 1) };
        return *alive[idx];
    }

    bool anyAlive() const {
        return std::any_of(members.begin(), members.end(),
            [](const auto& p) { return p->isAlive(); });
    }
    std::size_t aliveCount() const {
        return std::count_if(members.begin(), members.end(),
            [](const auto& p) { return p->isAlive(); });
    }
    Character& randomAliveTarget() {
        std::vector<Character*> alive;
        alive.reserve(members.size());
        for (auto& p : members)
            if (p->isAlive()) alive.push_back(p.get());

        if (alive.empty())
            throw std::runtime_error{ "No alive members" };

        const int idx{ randInt(0, static_cast<int>(alive.size()) - 1) };
        return *alive[idx];
    }

    void printRoster() const {
        std::cout << "\nГруппа " << name << "\n\n";
        for (const auto& m : members) {
            if (m->isAlive()) m->getStats();
            else std::cout << "  (мёртв) " << m->getName() << '\n';
        }
    }

    const std::string& getName() const { return name; }
};


int main()
{
    //std::cout << "Hello World!\n";
    setlocale(LC_ALL, "Russian");
    srand(time(0));

    Party heroes{ "Герои" };
    Party villains{ "Злодеи" };
    int nextId{ 1 };

    heroes.addMember(new Warrior(nextId++, "Warrior1", 120, 18, 10, "plate"));
    heroes.addMember(new Mage(nextId++, "Mage1", 80, 12, 5, 50));
    heroes.addMember(new Archer(nextId++, "Archer1", 90, 15, 7, 20));

    villains.addMember(new Warrior(nextId++, "Warrior2", 110, 17, 9, "chain"));
    villains.addMember(new Mage(nextId++, "Mage2", 75, 13, 4, 60));
    villains.addMember(new Archer(nextId++, "Archer2", 85, 16, 6, 25));

    std::cout << "Стартовые характеристики\n";
    heroes.printRoster();
    villains.printRoster();

    for (int turn = 1; turn <= TURNS; ++turn) {
        std::cout << "\nход " << turn << "\n";
        heroes.partyAttack(villains);
        if (!villains.anyAlive()) break;
        villains.partyAttack(heroes);
        if (!heroes.anyAlive()) break;

        if (turn % 2 == 0) {
            heroes.partyUseSkills(villains);
            villains.partyUseSkills(heroes);
        }
    }

    std::cout << "\nИтоговое состояние\n";
    heroes.printRoster();
    villains.printRoster();

    std::cout << "\n>>> Победитель: ";
    if (heroes.anyAlive() && !villains.anyAlive())
        std::cout << heroes.getName();
    else if (villains.anyAlive() && !heroes.anyAlive())
        std::cout << villains.getName();
    else
        std::cout << "ничья (лимит ходов)";
    std::cout << " <<<\n";

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