#include "hashtable.h"
#include <cstdlib>
#include <cmath>

hash_entry::hash_entry(char *str, int data) : my_data(data)
{
    my_string = new char[strlen(str) + 1];
    strcpy(my_string, str);
}

int bad_hashing::operator()(char *str, int N)
{
    if (str == NULL)
        return 0;
    int hash = 0;
    for (int i = 0; i < strlen(str); ++i)
        hash = (hash + str[i]) % N;
    return hash;
}
int good_hashing::operator()(char *str, int N)
{
    if (str == NULL)
        return 0;
    int hash = 0;
    for (int i = 0; i < strlen(str); ++i)
        hash = (hash * 151 + str[i]) % N;
    return hash;
}

void linear_probe::init() {}
int linear_probe::operator()(hash_entry *Table, int table_size, int last_choice)
{
    return (last_choice + 1) % table_size;
}

void twoway_quadratic_probe::init() { i = 0; }
int twoway_quadratic_probe::operator()(hash_entry *Table, int table_size, int last_choice)
{
    int sign = i % 2 == 0 ? -1 : 1;
    int _i = std::ceil(i / 2.0);
    int new_choice = (last_choice + sign * _i * _i) % table_size;
    if (new_choice < 0)
        new_choice += table_size; // 确保 new_choice 为非负数
    i++;
    if (i > table_size)
        return -1; // 如果探测次数超过哈希表大小，返回 -1
    return new_choice;
}
void common_overflow::init() {}
int common_overflow::operator()(hash_entry *Table, int table_size, int last_choice)
{
    for (int i = table_size; i < table_size + OVERFLOW_SIZE; ++i)
    {
        if (Table[i].status == hash_entry::EMPTY)
            return i;
    }
    return -1; // No empty slot found in overflow area
}

hashtable::hashtable(int size, hashing_strategy *hashing, collision_strategy *collision)
    : table_size(size), my_hashing(hashing), my_collision(collision)
{
    Table = new hash_entry[table_size];
}

bool hashtable::insert(hash_entry entry)
{
    int last_choice = (*my_hashing)(entry.my_string, table_size);
    my_collision->init();
    while (Table[last_choice].status == hash_entry::OCCUPIED) // loop infinitely? return false when no more space?
    {
        last_choice = (*my_collision)(Table,
                                      dynamic_cast<common_overflow *>(my_collision)
                                          ? table_size - OVERFLOW_SIZE
                                          : table_size,
                                      last_choice);
        if (last_choice == -1)
            return false;
    }
    Table[last_choice] = entry;
    Table[last_choice].status = hash_entry::OCCUPIED;
    return true;
}
int hashtable::query(char *query_string)
{
    int last_choice = (*my_hashing)(query_string, table_size);
    my_collision->init();
    while (Table[last_choice].status == hash_entry::OCCUPIED &&
           strcmp(Table[last_choice].my_string, query_string) != 0) // 未处理的情况: 哈希表已满?
    {
        last_choice = (*my_collision)(Table,
                                      dynamic_cast<common_overflow *>(my_collision)
                                          ? table_size - OVERFLOW_SIZE
                                          : table_size,
                                      last_choice);
        if (last_choice == -1)
            return -1;
    }
    if (Table[last_choice].status == hash_entry::EMPTY)
        return -1;
    else
        return Table[last_choice].my_data;
}