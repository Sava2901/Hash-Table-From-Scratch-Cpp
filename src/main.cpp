#include <iostream>
#include <utility>
#include "hash_table_fixed.hpp"
#include "hash_table_dynamic.hpp"

struct Student {
    std::string name;
    int age;
    Student(std::string n, int a) : name(std::move(n)), age(a) {}
    void print() const {
        std::cout << "Name: " << name << ", Age: " << age << std::endl;
    }
};

int main() {
    std::cout << "Testing Fixed-Type Hash Table:" << std::endl;
    HashTableFixed<int, Student> fixedTable(10);

    fixedTable.hashTableInsert(1, Student("Alice", 20));
    fixedTable.hashTableInsert(2, Student("Bob", 22));

    Student* student1 = fixedTable.hashTableSearch(1);
    if (student1) student1->print();

    fixedTable.hashTableDelete(1);
    student1 = fixedTable.hashTableSearch(1);
    if (!student1) std::cout << "Student with key 1 not found!" << std::endl;

    std::cout << std::endl;

    std::cout << "Testing Dynamic-Type Hash Table:" << std::endl;
    HashTable ht(10);

    std::string key1 = "apple";
    int value1 = 42;
    ht.insert(key1, value1);

    std::string key2 = "banana";
    int value2 = 100;
    ht.insert(key2, value2);

    auto retrieved_value1 = static_cast<int*>(ht.search(key1));
    if (retrieved_value1) {
        std::cout << "Found value for 'apple': " << *retrieved_value1 << std::endl;
    }

    ht.deleteKey(key1);

    retrieved_value1 = static_cast<int*>(ht.search(key1));
    if (!retrieved_value1) {
        std::cout << "'apple' not found after deletion." << std::endl;
    }

    HashTable table(100);

    table.insert(1, "One");
    table.insert(2, "Two");

    // table[3] = "Three";
    // std::cout << table[1] << std::endl;
    // std::cout << table[3] << std::endl;

    return 0;
}
