#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <mpi.h>

using namespace std;

int main(int argc, char *argv[])
{
    std::chrono::steady_clock::time_point time_begin = std::chrono::steady_clock::now();

    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    MPI_Status status;      // MPI receive
    MPI_Request request;    // MPI send
    
    fstream input_file("input-knapsack.txt");
    int N;
    int64_t Capacity;
    if (rank == 0)
        input_file >> N >> Capacity;
    MPI_Bcast(&N, 1, MPI_INT, 0, comm);
    MPI_Bcast(&Capacity, 1, MPI_LONG, 0, comm);
    MPI_Barrier(comm);

    int64_t weight[N], value[N];
    if (rank == 0)
        for (int i = 0; i < N; ++i)
            input_file >> weight[i] >> value[i];
    MPI_Bcast(weight, N, MPI_LONG, 0, comm);
    MPI_Bcast(value, N, MPI_LONG, 0, comm);
    MPI_Barrier(comm);


    vector<vector<int64_t>> dp(N + 1, vector<int64_t>(Capacity + 1));
    int64_t prev_max_value;    // mpi send and receive variable
    
    for (int i = 0; i <= N; ++i)    // for each item from 0 to n
    {
        for (int64_t j = rank; j <= Capacity; j += size)   // for each capacity from 0 to Capacity, each thread computes its own rows
        {
            if (i == 0 || j == 0)
                dp[i][j] = 0;
            else if (j < weight[i - 1])
                dp[i][j] = dp[i - 1][j];
            else
            {
                // int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status)
                MPI_Recv(&prev_max_value, 1, MPI_LONG, (j - weight[i - 1]) % size, i - 1, comm, &status);
                dp[i][j] = max(dp[i - 1][j], prev_max_value + value[i - 1]);
            }

            // send dp[i][j] to the next nodes that may need this curr_max_value
            if (i < N && weight[i] + j <= Capacity)
            {
                // int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
                MPI_Isend(&dp[i][j], 1, MPI_LONG, (j + weight[i]) % size, i, comm, &request);    // asynchronous operation
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (rank == Capacity % size)
        printf("max value: %ld\n", dp[N][Capacity]);

    if (rank == 0)
    {
        std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
        printf("time: %ld ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin).count());
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}