#include <cstdio>
#include <cstdlib>
int main(){
    system("g++ rand_input.cpp -o rand_input");
    system("g++ PA1-1-1.cpp -o PA1-1-1");
    system("g++ correct.cpp -o correct");
    while(1){
        printf("%s","1");
        system("./rand_input > rand.in");
        system("time ./PA1-1-1 < rand.in > 1.out");
        system("./correct < rand.in > 3.out");
        if(system("diff 1.out 3.out")!=0){
            printf("different output!\n");
            break;
        }
    }
    return 0;
}