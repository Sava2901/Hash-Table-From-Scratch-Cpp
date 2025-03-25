#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <functional>
#include <string>
#include <iostream>
#include <cstring>
#include <utility>
#include <memory>

class HashTable {
private:
    struct Item {
        std::unique_ptr<void, void(*)(void*)> key;
        std::unique_ptr<void, void(*)(void*)> value;
        size_t key_size;
        size_t value_size;
        Item* next;

        Item(void* k, const size_t ks, void* v, const size_t vs)
            : key(k, [](void* ptr) { delete[] static_cast<char*>(ptr); }),
              value(v, [](void* ptr) { delete[] static_cast<char*>(ptr); }),
              key_size(ks), value_size(vs), next(nullptr) {}
    };

    Item** items;
    size_t size;
    size_t count;

    std::function<unsigned long(const void*, size_t, size_t)> hash_function;
    std::function<int(const void*, const void*, size_t)> key_compare;

    static unsigned long defaultHashFunction(const void* key, size_t key_size, size_t table_size);
    static int defaultKeyCompare(const void* key1, const void* key2, size_t key_size);

public:
    explicit HashTable(size_t table_size = 100,
              std::function<unsigned long(const void*, size_t, size_t)> hash_func = nullptr,
              std::function<int(const void*, const void*, size_t)> key_comp = nullptr);
    ~HashTable();

    static Item* newItem(const void* key, size_t key_size, const void* value, size_t value_size);

    template <typename K, typename V>
    void insert(const K& key, const V& value);

    template <typename K, typename V>
    V& operator[](const K& key);

    template <typename K>
    void* search(const K& key);

    template <typename K>
    void deleteKey(const K& key);

    void print() const;
};

inline unsigned long HashTable::defaultHashFunction(const void* key, const size_t key_size, const size_t table_size) {
    const auto* data = static_cast<const unsigned char*>(key);
    unsigned long hash = 5381;
    for (size_t i = 0; i < key_size; ++i) {
        hash = (hash << 5) + hash + data[i];
    }
    return hash % table_size;
}

inline int HashTable::defaultKeyCompare(const void* key1, const void* key2, const size_t key_size) {
    return std::memcmp(key1, key2, key_size);
}

inline HashTable::HashTable(const size_t table_size,
                            std::function<unsigned long(const void*, size_t, size_t)> hash_func,
                            std::function<int(const void*, const void*, size_t)> key_comp)
    : size(table_size), count(0), hash_function(std::move(hash_func)), key_compare(std::move(key_comp)) {
    items = new Item*[size]();

    hash_function = hash_function ? hash_function : [this](const void* key, const size_t key_size, const size_t table_size) {
        return defaultHashFunction(key, key_size, table_size);
    };

    key_compare = key_compare ? key_compare : [this](const void* key1, const void* key2, const size_t key_size) {
        return defaultKeyCompare(key1, key2, key_size);
    };
}

inline HashTable::~HashTable() {
    for (size_t i = 0; i < size; ++i) {
        const Item* current = items[i];
        while (current) {
            const Item* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] items;
}

inline HashTable::Item* HashTable::newItem(const void* key, const size_t key_size, const void* value, const size_t value_size) {
    auto key_memory = std::make_unique<char[]>(key_size);
    auto value_memory = std::make_unique<char[]>(value_size);

    std::memcpy(key_memory.get(), key, key_size);
    std::memcpy(value_memory.get(), value, value_size);

    return new Item(key_memory.release(), key_size, value_memory.release(), value_size);
}


template <typename K, typename V>
void HashTable::insert(const K& key, const V& value) {
    const size_t key_size = sizeof(K);
    const size_t value_size = sizeof(V);

    const unsigned long index = hash_function(&key, key_size, size);
    const Item* current = items[index];

    while (current != nullptr) {
        if (key_compare(current->key.get(), &key, key_size) == 0) {
            std::memcpy(current->value.get(), &value, value_size);
            return;
        }
        current = current->next;
    }

    Item* new_item = newItem(&key, key_size, &value, value_size);
    new_item->next = items[index];
    items[index] = new_item;
    ++count;
}

template <typename K, typename V>
V& HashTable::operator[](const K& key) {
    const size_t key_size = sizeof(K);
    const size_t value_size = sizeof(V);

    const unsigned long index = hash_function(&key, key_size, size);
    const Item* current = items[index];

    while (current != nullptr) {
        if (key_compare(current->key.get(), &key, key_size) == 0) {
            return *static_cast<V*>(current->value.get());
        }
        current = current->next;
    }

    V default_value = V();
    Item* new_item = newItem(&key, key_size, &default_value, value_size);
    new_item->next = items[index];
    items[index] = new_item;
    ++count;

    return *static_cast<V*>(new_item->value.get());
}

template <typename K>
void* HashTable::search(const K& key) {
    const size_t key_size = sizeof(K);
    const unsigned long index = hash_function(&key, key_size, size);
    const Item* current = items[index];

    while (current != nullptr) {
        if (key_compare(current->key.get(), &key, key_size) == 0) {
            return current->value.get();
        }
        current = current->next;
    }

    return nullptr;
}

template <typename K>
void HashTable::deleteKey(const K& key) {
    const size_t key_size = sizeof(K);
    const unsigned long index = hash_function(&key, key_size, size);
    Item** current = &items[index];

    while (*current != nullptr) {
        if (key_compare((*current)->key.get(), &key, key_size) == 0) {
            const Item* temp = *current;
            *current = (*current)->next;
            delete temp;
            --count;
            return;
        }
        current = &((*current)->next);
    }
}

inline void HashTable::print() const {
    for (size_t i = 0; i < size; ++i) {
        const Item* current = items[i];
        while (current) {
            std::cout << "Key: " << *static_cast<std::string *>(current->key.get())
                      << " Value: " << *static_cast<int *>(current->value.get()) << std::endl;
            current = current->next;
        }
    }
}

#endif
