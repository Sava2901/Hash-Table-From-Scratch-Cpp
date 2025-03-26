#include <iostream>
#include <string>
#include "hash_table_dynamic.hpp"
#include "hash_table_fixed.hpp"

struct Student {
    int id;
    double score;
    char name[20];
};

int main() {
    HashTable dynamicTable(53);
    int intKey = 42;
    auto strKey = "dynamic";
    Student student{101, 95.5, "Alice"};
    float floatValue = 3.14f;

    dynamicTable[&intKey] = &student;
    dynamicTable[&strKey] = &floatValue;

    Student* s = dynamicTable[&intKey];
    if(s) std::cout << "Dynamic Student: " << s->name << " ID: " << s->id << "\n";
    float* f = dynamicTable[&strKey];
    if(f) std::cout << "Dynamic Float: " << *f << "\n";



    HashTableFixed<std::string, int> fixedTable;
    fixedTable.Insert("Alice", 90);
    fixedTable.Insert("Bob", 85);
    fixedTable["Charlie"] = 95;

    std::cout << "Alice: " << *fixedTable.Search("Alice") << "\n";
    std::cout << "Bob: " << fixedTable["Bob"] << "\n";
    std::cout << "Charlie: " << fixedTable["Charlie"] << "\n";

    fixedTable.Delete("Bob");
    if(!fixedTable.Contains("Bob")) std::cout << "Bob deleted\n";

    fixedTable["Alice"] = 100;
    std::cout << "Updated Alice: " << fixedTable["Alice"] << "\n";

    return 0;
}