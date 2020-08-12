#include <iostream>
#include <fstream>
#include <openssl/rand.h>

using namespace std;

int main (int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "usage: %s N C\n", argv[0]);
        exit(1);
    }
    
    int N = stoi (argv[1]);
    uint64_t C = stoi (argv[2]);
    
    int m = 4 * C / N;
    unsigned char buff[2 * N];
    RAND_seed(&m, sizeof(m));
    RAND_bytes(buff, sizeof(buff));

    
    ofstream file_stream;
    file_stream.open("input-knapsack.txt");
    file_stream << N << ' ' << C << endl;
    for (int i = 0; i < N; i++)
    {
        file_stream << buff[2 * i] % m << ' ' << buff[2 * i + 1] % m << endl;;
    }
    file_stream.close();

    return 0;
}