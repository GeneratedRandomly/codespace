#include <cstdio>
#include <ctime>
#include "hashtable.h"

hashing_strategy *get_hashing_strategy(const char *name)
{
    if (strcmp(name, "bad") == 0)
        return new bad_hashing();
    if (strcmp(name, "good") == 0)
        return new good_hashing();
    return nullptr;
}

collision_strategy *get_collision_strategy(const char *name)
{
    if (strcmp(name, "linear") == 0)
        return new linear_probe();
    if (strcmp(name, "quadratic") == 0)
        return new twoway_quadratic_probe();
    if (strcmp(name, "overflow") == 0)
        return new common_overflow(); // 假设溢出区大小为1000
    return nullptr;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <hashing_strategy> <collision_strategy>\n", argv[0]);
        return 1;
    }
    hashing_strategy *hashing = get_hashing_strategy(argv[1]);
    collision_strategy *collision = get_collision_strategy(argv[2]);
    if (!hashing || !collision)
    {
        printf("Invalid strategy names.\n");
        return 1;
    }
    int type;
    char buffer[1000];
    int data;
    hashtable table(TABLE_SIZE, hashing, collision);
    while (true)
    {
        scanf("%d", &type);
        if (type == 0)
        {
            scanf("%s", buffer);
            scanf("%d", &data);
            table.insert(hash_entry(buffer, data));
        }
        else if (type == 1)
        {
            scanf("%s", buffer);
            printf("%d\n", table.query(buffer));
        }
        else
            break;
    }
    return 0;
}