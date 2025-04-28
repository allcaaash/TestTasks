#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include <codecvt>

#include <algorithm>

struct Person
{
    std::wstring surname;
    std::wstring name;
    std::wstring phone;
};

std::vector<Person> _persons;

bool ReadFile()
{
    std::wifstream file("input.txt", std::ios::in);
    file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t, 0x10FFFF, std::consume_header>));

    if (!file.is_open())
        return false;

    bool isCorrectFormat = true;
    std::wstring line; // пример входной строки "Иванов Иван: 3515454"
    while (std::getline(file, line))
    {
        Person person;
        size_t pos = line.find(L':');
        if (pos == std::string::npos)
        {
            isCorrectFormat = false;
            break;
        }

        std::wstring name_surname = line.substr(0, pos).c_str();
        person.phone = line.substr(pos + 2);

        pos = name_surname.find(L' ');
        if (pos == std::string::npos)
        {
            isCorrectFormat = false;
            break;
        }

        person.surname = name_surname.substr(0, pos);
        person.name = name_surname.substr(pos + 1);

        _persons.push_back(person);
    }
    file.close();

    return isCorrectFormat;
}

bool CompareByName(const Person& first, const Person& second)
{
    if (first.name == second.name)
    {
        if (first.surname == second.surname)
            return first.phone < second.phone;
    }
    return first.name < second.name;
}

bool CompareBySurname(const Person& first, const Person& second)
{
    if (first.surname == second.surname)
    {
        if (first.name == second.name)
            return first.phone < second.phone;
    }
    return first.surname < second.surname;
}

bool CompareByPhone(const Person& first, const Person& second)
{
    return first.phone > second.phone;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    if (!ReadFile())
    {
        std::cout << "Ошибка чтения файла" << std::endl;
        return 1;
    }

    std::cout << "Для сортировки по именам введите – 1, по фамилиям введите - 2, по телефонам введите - 3. Для завершения программы - 0" << std::endl;
    char ch;
    std::cin >> ch;
    std::cout << std::endl;

    if (!std::isdigit(ch))
    {
        printf("Ошибка! \'%c\' не является числом", ch);
        return 1;
    }

    int choise = ch - '0';

    switch (choise)
    {
    case 1:
        std::sort(_persons.begin(), _persons.end(), CompareByName);
        for (const auto& person : _persons)
            std::wcout << person.name << " " << person.surname << ": " << person.phone << std::endl;
        break;
    case 2:
        std::sort(_persons.begin(), _persons.end(), CompareBySurname);
        for (const auto& person : _persons)
            std::wcout << person.surname << " " << person.name << ": " << person.phone << std::endl;
        break;
    case 3:
        std::sort(_persons.begin(), _persons.end(), CompareByPhone);
        for (const auto& person : _persons)
            std::wcout << person.phone << ": " << person.surname << " " << person.name << std::endl;
        break;
    default:
        std::cout << "Ошибка! Данному номеру не сответствует ни один из предложенных вариантов" << std::endl;
        return 1;
        break;
    }

    return 0;
}