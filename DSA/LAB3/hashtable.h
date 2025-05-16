#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <cstring>
#include <cstdio>
const int TABLE_SIZE = 499883; // = 124970 * 4 + 3
const int OVERFLOW_SIZE = 100000;
struct hash_entry
{
    char *my_string = nullptr;
    int my_data = 0;
    int status = EMPTY;
    enum { EMPTY, OCCUPIED };
    hash_entry(){}
    hash_entry(char *str, int data);
};

struct hashing_strategy
{
    virtual int operator()(char *str, int N) = 0;
};
struct bad_hashing : public hashing_strategy
{
    int operator()(char *str, int N) override;
};
struct good_hashing : public hashing_strategy
{
    int operator()(char *str, int N) override;
};

struct collision_strategy
{
    virtual void init() = 0; // pure virtual function
    virtual int operator()(hash_entry *Table, int table_size, int last_choice) = 0;
};
struct linear_probe : public collision_strategy
{
    void init();
    int operator()(hash_entry *Table, int table_size, int last_choice) override;
};
struct twoway_quadratic_probe : public collision_strategy
{
    void init() override;
    int operator()(hash_entry *Table, int table_size, int last_choice) override;
private:
    int i;
};
struct common_overflow : public collision_strategy
{
    void init() override;
    int operator()(hash_entry *Table, int table_size, int last_choice) override;
};

struct hashtable
{
    hash_entry *Table;
    int table_size;
    hashing_strategy *my_hashing; // 如果改为hashing_strategy my_hashing, 即不是用指针作为hashtable的成员, 而是让hashing_strategy结构体直接作为hashtable的成员. 会发生什么bug?
    collision_strategy *my_collision;
    hashtable(int size, hashing_strategy *hashing, collision_strategy *collision);
    bool insert(hash_entry entry);
    int query(char *query_string);
};
#endif