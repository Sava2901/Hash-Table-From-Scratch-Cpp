#ifndef HASH_TABLE_FIXED_HPP
#define HASH_TABLE_FIXED_HPP

#include <iostream>
#include <string>

template <typename K>
struct CustomHash {
    size_t operator()(const K& key) const {
        size_t hash = 0;
        const char* data = reinterpret_cast<const char*>(&key);
        for (size_t i = 0; i < sizeof(K); ++i) {
            hash = hash * 31 + data[i];
        }
        return hash;
    }
};

template <>
struct CustomHash<std::string> {
    size_t operator()(const std::string& key) const {
        size_t hash = 0;
        for (char ch : key) {
            hash = hash * 31 + ch;
        }
        return hash;
    }
};

template <typename K, typename V>
class HashTableFixed {
private:
    struct Item {
        K key;
        V value;
        Item(K k, V v) : key(k), value(v) {}
    };

    Item** items;
    size_t size;
    size_t capacity;

    size_t hash(const K& key) const {
        return CustomHash<K>{}(key) % capacity;
    }

public:
    explicit HashTableFixed(const size_t capacity = 100) : size(0), capacity(capacity) {
        items = new Item*[capacity]();
    }

    ~HashTableFixed() {
        for (size_t i = 0; i < capacity; ++i) {
            if (items[i]) {
                delete items[i];
            }
        }
        delete[] items;
    }

    void hashTableInsert(const K& key, const V& value) {
        size_t index = hash(key);
        while (items[index]) {
            if (items[index]->key == key) {
                items[index]->value = value;
                return;
            }
            index = (index + 1) % capacity;
        }
        items[index] = new Item(key, value);
        ++size;
    }

    V* hashTableSearch(const K& key) {
        size_t index = hash(key);
        while (items[index]) {
            if (items[index]->key == key) {
                return &items[index]->value;
            }
            index = (index + 1) % capacity;
        }
        return nullptr;
    }

    void hashTableDelete(const K& key) {
        size_t index = hash(key);
        while (items[index]) {
            if (items[index]->key == key) {
                delete items[index];
                items[index] = nullptr;
                --size;
                return;
            }
            index = (index + 1) % capacity;
        }
    }
};

#endif
