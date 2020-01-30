#include <iostream>
#include <vector>
#include <fstream>

#define ull unsigned long long int
#define SIZE (4*1024/8)


static inline unsigned long long rdtsc()
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long) lo) | (((unsigned long long) hi) << 32);
}

volatile ull *direct(ull N, ull offset)
{
    volatile ull *arr = new ull[N]();

    ull step = 0;
    ull i, temp;

    for (i = 0; i < N; i = i + offset)
    {
        temp = i + step;
        step = (step + 8)%SIZE;

        arr[temp] = (i + offset + step) % N;
    }

    arr[temp] = 0;

    return arr;
}

ull pass(const volatile ull *arr, ull N, int repeat, int check)
{
    ull next = 0;
    ull volatile t1, t2;
    ull min_time = ULLONG_MAX;

    for (ull i = 0; i < (N / SIZE); i++)
    {
        next = arr[next];
    }

    for (int k = 0; k < check; k++)
    {
        t1 = rdtsc();
        for (ull i = 0; i < (N / SIZE) * repeat; i++)
        {
            next = arr[next];
        }
        t2 = rdtsc();

        if (min_time > t2 - t1)
        {
            min_time = t2 - t1;
        }
    }

    return min_time;
}

int power_of_two(ull Value)
{
    ull InitValue = 1;
    int power = 0;

    while (InitValue < Value)
    {
        power++;
        InitValue *= 2;
        if (InitValue == Value)
            return power;
    }

    return 0;
}

int main()
{

    std::ofstream out;
    out.open("/Users/twiggy/Desktop/output.txt");

    out << "Count" << "\t" << "Size" << "\t" << "Time" << std::endl;

    int repeat = 10;

    for (ull cur_N = SIZE; cur_N <= SIZE * 100000; cur_N *= 2)
    {
        ull temp_N = cur_N;

        while (temp_N < 2 * cur_N)
        {
            out << temp_N / (SIZE) << "\t" << power_of_two((temp_N / (SIZE))*8) << "\t";

            ull time = 0;
            volatile ull *arr_d = direct(temp_N, SIZE);

            time = pass(arr_d, temp_N, repeat, 10) / (temp_N / SIZE) / repeat;

            out << time << std::endl;
            delete[] arr_d;

            temp_N += cur_N / 4;
        }
    }

    out.close();

    return 0;

}

/*
 * Sizeof(PAGE) = 4кб = (4 * 1024) байт
 * В одну страницу вмещается 512 элементов ull
 */

/*
volatile ull *permutation(ull N) //Алгоритм Саттоло
{
    srand(time(NULL));

    volatile ull *arr = new ull[N]();

    for (ull i = 0; i < N; i++)
    {
        arr[i] = i;
    }

    for (ull i = N - 1; i > 0; i--)
    {
        ull j = rand() % i;

        ull temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    return arr;
}

volatile ull *rand(ull N, ull offset)
{
    volatile ull *perm = permutation(N / offset);
    volatile ull *arr = new ull[N]();

    for (ull i = 0; i < N / offset; i++)
    {
        arr[i * offset] = (perm[i] * offset) % N;
    }

    delete[] perm;
    return arr;
}
*/