// lab 8.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <memory>
#include <algorithm>

using namespace std;

class IClient {
public:
	virtual ~IClient() = default;
	virtual int getId() const = 0;
	virtual string getName() const = 0;
	virtual string getContacts() const = 0;
};


class Client : public IClient {
private:
	int id;
	string name;
	string contacts;

public:
	Client(int id, string name, string contacts)
		: id(id), name(name), contacts(contacts) { }
	int getId() const override { return id; }
	string getName() const override { return name; }
	string getContacts() const override { return contacts; }
};


class IDeal {
public:
	virtual ~IDeal() = default;
	virtual int getId() const = 0;
	virtual int getClientId() const = 0;
	virtual double getAmount() const = 0;
	virtual void print() const = 0;
};


template <typename T>
class Deal : public IDeal {
private:
	int id;
	int clientId;
	double amount;
	T product;

public:
	Deal(int id, int clientId, double amount, T product)
		: id(id), clientId(clientId), amount(amount), product(product) { }
	int getId() const override { return id; }
	int getClientId() const override { return clientId; }
	double getAmount() const override { return amount; }
	T getProduct() const { return product; }

	void print() const override {
		cout << "ID сделки: " << id << "\n";
		cout << "ID клиента: " << clientId << "\n";
		cout << "Сумма: " << amount << "\n";
		cout << "Продукт: " << product << "\n";
	}
};


class Pipeline {
private:
	map<string, vector<int>> stages;
	unordered_map<int, string> dealStages;

public:
	void moveDeal(int dealId, const string& newStage) {
		auto i = dealStages.find(dealId);
		if (i != dealStages.end()) {
			string oldStage = i->second;
			auto& oldDeals = stages[oldStage];
			oldDeals.erase(
				remove(oldDeals.begin(), oldDeals.end(), dealId),
				oldDeals.end()
			);
		}
		stages[newStage].push_back(dealId);
		dealStages[dealId] = newStage;
	}

	bool getDealStage(int dealId, string& result) const {
		auto i = dealStages.find(dealId);
		if (i == dealStages.end()) return false;
		result = i->second;
		return true;
	}

	void printPipeline() const {
		cout << "\nСтадии сделок:\n";
		for (const auto& stage : stages) {
			cout << stage.first << ": ";
			for (int dealId : stage.second) {
				cout << dealId << " ";
			}
			cout << endl;
		}
	}
};

class ClientRepo {
private:
	unordered_map<int, shared_ptr<IClient>> clients;

public:
	bool addClient(shared_ptr<IClient> client) {
		if (client == nullptr) return false;
		int id = client->getId();
		if (clients.find(id) != clients.end()) return false;
		clients[id] = client;
		return true;
	}

	shared_ptr<IClient> findClient(int id) const {
		auto i = clients.find(id);
		if (i == clients.end()) return nullptr;
		return i->second;
	}
};


class DealRepo {
private:
	vector<shared_ptr<IDeal>> deals;
	unordered_map<int, shared_ptr<IDeal>> dealsById;

public:
	bool addDeal(shared_ptr<IDeal> deal) {
		if (deal == nullptr) return false;
		int id = deal->getId();
		if (dealsById.find(id) != dealsById.end()) return false;
		deals.push_back(deal);
		dealsById[id] = deal;
		return true;
	}

	shared_ptr<IDeal> findDeal(int id) const {
		auto i = dealsById.find(id);
		if (i == dealsById.end()) return nullptr;
		return i->second;
	}

	bool contains(int id) {
		return dealsById.find(id) != dealsById.end();
	}

	vector<shared_ptr<IDeal>> findDealsByClient(int clientId) const {
		vector<shared_ptr<IDeal>> result;
		for (auto deal : deals) {
			if (deal->getClientId() == clientId) result.push_back(deal);
		}
		return result;
	}

	const vector<shared_ptr<IDeal>>& getAllDeals() const { return deals; }
};


class CRM {
private:
	ClientRepo clientRepo;
	DealRepo dealRepo;
	Pipeline pipeline;

public:
	bool addClient(shared_ptr<IClient> client) {
		return clientRepo.addClient(client);
	}

	template <typename T>
	bool addDeal(int id, int clientId, double amount, T product) {
		if (clientRepo.findClient(clientId) == nullptr) {
			cout << "Клиент не найден\n";
			return false;
		}
		auto deal = make_shared<Deal<T>>(id, clientId, amount, product);
		if (!dealRepo.addDeal(deal)) {
			cout << "Сделка с таким ID уже существует\n";
			return false;
		}
		pipeline.moveDeal(id, "Новая");
		return true;
	}

	shared_ptr<IDeal> findDeal(int id) const {
		return dealRepo.findDeal(id);
	}

	vector<shared_ptr<IDeal>> findClientDeals(int clientId) const {
		return dealRepo.findDealsByClient(clientId);
	}

	bool changeDealStage(int dealId, const string& newStage) {
		if (!dealRepo.contains(dealId)) {
			return false;
		}
		pipeline.moveDeal(dealId, newStage);
		return true;
	}

	const Pipeline& getPipeline() const {
		return pipeline;
	}

	void printDeal(int id) const {
		auto deal = dealRepo.findDeal(id);
		if (deal == nullptr) {
			cout << "Сделка не найдена\n";
			return;
		}
		deal->print();
	}
};



int main() {
	setlocale(LC_ALL, "Russian");

	CRM crm;
	auto client1 = make_shared<Client>(1, "Петя", "petya@gmail.com");
	auto client2 = make_shared<Client>(2, "Маша", "masha@gmail.com");
	crm.addClient(client1);
	crm.addClient(client2);

	crm.addDeal(101, 1, 60000.0, string("Ноутбук"));
	crm.addDeal(102, 1, 3000.0, string("Клавиатура"));
	crm.addDeal(103, 2, 5000.0, string("Монитор"));

	cout << "Поиск сделки:\n";
	crm.printDeal(102);

	cout << "\nСделки клиента с ID 1:\n";
	auto clientDeals = crm.findClientDeals(1);
	for (auto deal : clientDeals) {
		deal->print();
	}
	crm.changeDealStage(101, "Переговоры");
	crm.changeDealStage(102, "Завершена");
	crm.changeDealStage(103, "Новая");

	crm.getPipeline().printPipeline();

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
