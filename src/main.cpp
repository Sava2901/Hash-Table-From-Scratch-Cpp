#include "hash_table.hpp"
#include <iostream>
#include <string>

struct Person {
    std::string name;
    int age;

    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
};

int main() {
    HashTable<std::string, int> fruitTable;

    fruitTable["apple"] = 99;
    std::cout << fruitTable["apple"] << std::endl;

    fruitTable.Insert("apple", 1);
    std::cout << fruitTable["apple"] << std::endl;

    fruitTable.Insert("banana", 2);
    fruitTable.Insert("orange", 3);

    std::cout << "apple: " << fruitTable["apple"] << std::endl;
    std::cout << "banana: " << fruitTable["banana"] << std::endl;
    std::cout << "orange: " << fruitTable["orange"] << std::endl;
    std::cout << "berry: " << fruitTable["berry"] << std::endl;

    if (fruitTable.Contains("banana")) {
        std::cout << "banana is present in the table." << std::endl;
    }

    fruitTable.Delete("apple");
    if (!fruitTable.Contains("apple")) {
        std::cout << "apple was removed from the table." << std::endl;
    }

    HashTable<int, Person> idTable;
    idTable.Insert(1, Person{"Alice", 30});
    idTable.Insert(2, Person{"Bob", 25});

    idTable[3] = Person{"Charlie", 22};

    std::cout << "ID 1: " << idTable[1].name << ", " << idTable[1].age << std::endl;
    std::cout << "ID 2: " << idTable[2].name << ", " << idTable[2].age << std::endl;
    std::cout << "ID 3: " << idTable[3].name << ", " << idTable[3].age << std::endl;

    Person* person1 = idTable.Search(2);
    if (person1) {
        std::cout << "Found person with ID 2: " << person1->name << ", " << person1->age << std::endl;
    }

    Person* person2 = &idTable[3];
    if (person2) {
        std::cout << "Found person with ID 3: " << person2->name << ", " << person2->age << std::endl;
    }

    if (idTable.Contains(1)) {
        std::cout << "Person with ID 1 exists in the table." << std::endl;
    }

    HashTable<int, std::string> table;
    table[1] = "first";
    std::cout << table[1] << std::endl;
    std::cout << table[2] << std::endl;

    return 0;
}
