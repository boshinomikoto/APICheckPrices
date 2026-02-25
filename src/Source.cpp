#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <iomanip>
#include <vector>
#include <sstream>
#include <Windows.h>

#include <nlohmann/json.hpp> //add library
#include <Python.h>


using json = nlohmann::json;

// Базовый класс для общих утилит
class BaseReader 
{
protected:
    json objJson;

    std::string setColor(double num, int precision = 3) 
    {
        std::ostringstream ss{};
        ss << std::fixed << std::setprecision(precision);
        if (num > 0) ss << "\033[32m" << num << "\033[0m"; // Зеленый
        else if (num < 0) ss << "\033[31m" << num << "\033[0m"; // Красный
        else ss << num;
        return ss.str();
    }

public:
    void pyRun(const std::string& filename)
    {
        FILE* fp = fopen(filename.c_str(), "r");
        if (fp)
        {
            PyRun_SimpleFile(fp, filename.c_str());
            fclose(fp);
        }
        else
            std::cerr << "Cannot open Python file: " << filename << std::endl;
    }

    bool loadJson(const std::string& filename) 
    {
        std::ifstream fin(filename);
        if (!fin.is_open()) return false;
        fin >> objJson;
        return true;
    }
};

class BinanceReader : public BaseReader 
{
public:
    void printData() 
    {
        for (auto& [coin, dataArray] : objJson.items()) 
        {
            if (dataArray.empty()) continue;

            auto item = dataArray[0];
            double price = std::stod(item["price"].get<std::string>());
            double change = std::stod(item["change"].get<std::string>());
            double percent = std::stod(item["changePercent"].get<std::string>());

            std::cout << std::left << std::setw(6) << coin << " : "
                << std::setw(8) << item["symbol"].get<std::string>() << " -> "
                << std::right << std::fixed << std::setprecision(5)
                << std::setw(12) << price << "    "
                << std::setw(20) << setColor(change) << " $    "
                << std::setw(15) << setColor(percent) << " %" << std::endl;
        }
    }
};

class BankReader : public BaseReader {
public:
    void printData() {
        for (auto& [token, currencyData] : objJson.items()) {
            if (currencyData.empty()) continue;

            std::string bank = currencyData["bank"];
            double value = currencyData["value"];

            std::cout << std::left << std::setw(6) << bank << " : "
                << std::setw(8) << token << " -> "
                << std::right << std::setw(16) << setColor(value) << std::endl;
        }
    }
};

class Scheduler {
private:
    std::string path;
    int lastTotalMinutes = -1;

public:
    Scheduler(const std::string& p) : path(p) {}

    bool shouldUpdate() {
        std::ifstream fin(path);
        if (!fin.is_open()) return true;

        int h, m;
        if (fin >> h >> m) {
            lastTotalMinutes = h * 60 + m;
        }
        fin.close();

        time_t t = time(nullptr);
        struct tm* now = localtime(&t);
        int currentMinutes = now->tm_hour * 60 + now->tm_min;

        if (lastTotalMinutes == -1) return true;

        int diff = currentMinutes - lastTotalMinutes;
        if (diff < 0) diff += 1440; 

        return diff >= 119;
    }

    void saveTime() 
    {
        time_t t = time(nullptr);
        struct tm* now = localtime(&t);
        std::ofstream fout(path);
        if (fout.is_open()) 
        {
            fout << now->tm_hour << " " << now->tm_min;
        }
    }

    void printCurrentTime() 
    {
        time_t t = time(nullptr);
        struct tm* now = localtime(&t);
        std::cout << "Date: " << now->tm_mday << "." << now->tm_mon + 1 << "." << now->tm_year + 1900 << "\n";
        std::cout << "Time: " << std::setfill('0') << std::setw(2) << now->tm_hour << ":"
            << std::setw(2) << now->tm_min << std::setfill(' ') << "\n";
    }
};

int main() 
{
    SetConsoleOutputCP(CP_UTF8);
    Py_Initialize();

    BinanceReader bReader;
    BankReader bankReader;
    Scheduler scheduler("scripts/bankCurrency.txt");

    bankReader.loadJson("scripts/getCurrency.json");

    while (true) 
    {
        bReader.pyRun("scripts/getPrice.py");
        bReader.loadJson("scripts/getPrice.json");

        if (scheduler.shouldUpdate()) 
        {
            bankReader.pyRun("scripts/bankCurrency.py");
            bankReader.loadJson("scripts/getCurrency.json");
            scheduler.saveTime();
        }

        system("cls");
        scheduler.printCurrentTime();
        std::cout << "\n--- Binance Prices ---\n";
        bReader.printData();

        std::cout << "\n--- Bank Currency ---\n";
        bankReader.printData();

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    Py_Finalize();
    return 0;
}