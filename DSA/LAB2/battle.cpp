#include <cstdio>
#include <cstdlib>
int main()
{
    system("g++ rand_input.cpp -o rand_input");
    system("g++ check_input.cpp -o check_input");
    system("g++ 07.cpp -o test");
    //while (1)
    //{
        //system("./rand_input > rand.in");
        if (system("./check_input < 07.in") != 0){
            printf("invalid input!\n");
            //break;
        }   
        system("./test < 07.in > test.out");
        system("./check_input < 07.in > check_input.out");
        if (system("diff test.out check_input.out") != 0){
            printf("different output!\n");
            //break;
        }
    //}
    return 0;
}