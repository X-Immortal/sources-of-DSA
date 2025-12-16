//
// Created by xyx on 2025/12/11.
//
#include <iostream>
#include <string>
#include <windows.h>

using std::cout;
using std::endl;
using std::string;

namespace Hash {
    template<class K>
    concept HashKey = requires(K &a, K &b)
    {
        { a == b } -> std::convertible_to<bool>;
        { std::hash<K>()(a) } -> std::convertible_to<std::size_t>;
    };

    template<HashKey K, class V>
    class HashTable {
        struct Entry {
            int hash; // 哈希码
            K key;
            V value;
            Entry *next;

            template<class TK, class TV>
                requires std::constructible_from<K, TK&&> && std::constructible_from<V, TV&&>
            Entry(int hash, TK &&key, TV &&value, Entry *next = nullptr) :
                hash(hash), key(std::forward<TK>(key)), value(std::forward<TV>(value)), next(next) {
            }
        };

        friend int main();

        int m_size = 0;
        int capacity = 16; // 数组长度为2的幂，可以将一些运算转化为位运算，提高效率
        Entry **table = new Entry *[capacity](nullptr);
        const double load_factor = 0.75; // 负载因子
        int threshold = capacity * load_factor; // 扩容阈值

    public:
        HashTable() = default;

        ~HashTable() {
            for (int i = 0; i < capacity; i++) {
                Entry *entry = table[i];
                while (entry != nullptr) {
                    Entry *next = entry->next;
                    delete entry;
                    entry = next;
                }
            }
        }

        V &get(const K &key) {
            return get(std::hash<K>()(key), key);
        }

        template<class TK, class TV>
            requires std::constructible_from<K, TK&&> && std::constructible_from<V, TV&&>
        void put(TK &&key, TV &&value) {
            return put(std::hash<K>()(key), std::forward<TK>(key), std::forward<TV>(value));
        }

        V remove(const K &key) {
            return remove(std::hash<K>()(key), key);
        }

        int size() const {
            return m_size;
        }

    private:
        int hash(const K &key) {
            int hash = std::hash<K>()(key);
            // 高低位异或，使低位部分包含高位信息，减少数组长度很小时发生冲突的几率
            return hash ^ (hash >> 16);
        }

        V &get(int hash, const K &key) {
            int index = hash & capacity - 1;
            if (table[index] == nullptr) {
                throw std::runtime_error("empty entry!");
            }

            Entry *entry = table[index];
            while (entry != nullptr) {
                if (entry->key == key) {
                    return entry->value;
                }
                entry = entry->next;
            }
            throw std::runtime_error("no such key!");
        }

        template<class TK, class TV>
            requires std::constructible_from<K, TK&&> && std::constructible_from<V, TV&&>
        void put(int hash, TK &&key, TV &&value) {
            int index = hash & capacity - 1;
            if (table[index] == nullptr) {
                table[index] = new Entry(hash, std::forward<TK>(key), std::forward<TV>(value));
            } else {
                Entry *entry = table[index];
                while (true) {
                    if (entry->key == key) {
                        entry->value = std::forward<TV>(value);
                        return;
                    }
                    if (entry->next == nullptr) {
                        entry->next = new Entry(hash, std::forward<TK>(key), std::forward<TV>(value));
                        break;
                    }
                    entry = entry->next;
                }
            }

            m_size++;
            if (m_size > threshold) {
                expand();
            }
        }

        V remove(int hash, const K &key) {
            int index = hash & capacity - 1;
            if (table[index] == nullptr) {
                throw std::runtime_error("empty entry!");
            }

            Entry *entry = table[index];
            Entry *prev = nullptr;
            while (entry != nullptr) {
                if (entry->key == key) {
                    if (prev == nullptr) {
                        table[index] = entry->next;
                    } else {
                        prev->next = entry->next;
                    }
                    m_size--;
                    break;
                }
                prev = entry;
                entry = entry->next;
            }

            if (entry == nullptr) {
                throw std::runtime_error("no such key!");
            }

            V ret = entry->value;
            delete entry;
            return ret;
        }

        void expand() {
            Entry **new_table = new Entry *[capacity << 1](nullptr);
            for (int i = 0; i < capacity; i++) {
                Entry *entry = table[i];
                Entry *list1 = nullptr, *list2 = nullptr;
                Entry *p1 = nullptr, *p2 = nullptr;
                while (entry != nullptr) {
                    if ((entry->hash & capacity) == 0) {
                        if (list1 == nullptr) {
                            list1 = entry;
                        } else {
                            p1->next = entry;
                        }
                        p1 = entry;
                    } else {
                        if (list2 == nullptr) {
                            list2 = entry;
                        } else {
                            p2->next = entry;
                        }
                        p2 = entry;
                    }
                    entry = entry->next;
                }

                if (p1 != nullptr) {
                    p1->next = nullptr;
                    new_table[i] = list1;
                }
                if (p2 != nullptr) {
                    p2->next = nullptr;
                    new_table[i | capacity] = list2;
                }
            }

            delete[] table;
            table = new_table;
            capacity <<= 1;
            threshold = capacity * load_factor;
        }
    };
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    return 0;
}
