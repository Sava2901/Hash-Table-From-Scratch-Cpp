#ifndef HASH_TABLE_DYNAMIC_HPP
#define HASH_TABLE_DYNAMIC_HPP

#include <vector>
#include <cstring>

class HashTable {
private:
    struct Item {
        void* key;
        void* value;
        size_t key_size;
        size_t value_size;
        Item* next;

        Item(const void* k, size_t ks, const void* v, size_t vs)
            : key_size(ks), value_size(vs), next(nullptr) {
            key = new char[ks];
            std::memcpy(key, k, ks);
            value = new char[vs];
            std::memcpy(value, v, vs);
        }

        ~Item() {
            delete[] static_cast<char*>(key);
            delete[] static_cast<char*>(value);
        }
    };

    template <typename K>
    struct AssignmentHandler {
        HashTable& ht;
        const K* key;

        AssignmentHandler(HashTable& ht, const K* key) : ht(ht), key(key) {}

        template <typename V>
        void operator=(const V* value) {
            ht._insert(key, value);
        }

        template <typename T>
        operator T*() const {
            return static_cast<T*>(ht._search(key));
        }
    };

    int table_size;
    std::vector<Item*> items;

    using HashFunction = unsigned long (*)(const void*, size_t, int);
    using KeyCompareFunction = int (*)(const void*, const void*, size_t);

    HashFunction hash_function;
    KeyCompareFunction key_compare;

    static unsigned long defaultHashFunction(const void* key, size_t key_size, int size) {
        const auto* data = static_cast<const unsigned char*>(key);
        unsigned long hash = 5381;
        for (size_t i = 0; i < key_size; ++i) {
            hash = (hash << 5) + hash + data[i];
        }
        return hash % size;
    }

    static int defaultKeyCompare(const void* key1, const void* key2, size_t key_size) {
        return std::memcmp(key1, key2, key_size);
    }

    template <typename K, typename V>
    void _insert(const K* key, const V* value) {
        unsigned long index = hash_function(key, sizeof(K), table_size);
        Item* current = items[index];

        while (current) {
            if (key_compare(current->key, key, sizeof(K)) == 0) {
                delete[] static_cast<char*>(current->value);
                current->value = new char[sizeof(V)];
                std::memcpy(current->value, value, sizeof(V));
                current->value_size = sizeof(V);
                return;
            }
            current = current->next;
        }

        Item* new_item = new Item(key, sizeof(K), value, sizeof(V));
        new_item->next = items[index];
        items[index] = new_item;
    }

    template <typename K>
    void* _search(const K* key) const {
        unsigned long index = hash_function(key, sizeof(K), table_size);
        Item* current = items[index];

        while (current) {
            if (key_compare(current->key, key, sizeof(K)) == 0) {
                return current->value;
            }
            current = current->next;
        }
        return nullptr;
    }

public:
    explicit HashTable(int size, HashFunction hf = nullptr, KeyCompareFunction kc = nullptr)
        : table_size(size), items(size, nullptr),
          hash_function(hf ? hf : defaultHashFunction),
          key_compare(kc ? kc : defaultKeyCompare) {}

    ~HashTable() {
        for (auto& bucket : items) {
            Item* current = bucket;
            while (current) {
                Item* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    template <typename K>
    AssignmentHandler<K> operator[](const K* key) {
        return AssignmentHandler<K>(*this, key);
    }
};

#endif