// MAIN 部分修改
#include <chrono>
#include <fstream>
#include "md5.h"
#include "PCFG_multi_process.hpp"
#include <iomanip>
#include <unordered_set>
#include <iostream>
using namespace std;
using namespace chrono;

void run_process(int rank, int size) {
    double time_hash = 0;
    double time_guess = 0;
    double time_train = 0;

    PriorityQueue_mpi q;

    auto start_train = system_clock::now();
    // 所有进程使用相同的随机种子
    srand(42); // 固定种子保证一致性
    q.m.train("./input/Rockyou-singleLined-full.txt");
    q.m.order();
    auto end_train = system_clock::now();
    auto duration_train = duration_cast<microseconds>(end_train - start_train);
    time_train = double(duration_train.count()) * microseconds::period::num / microseconds::period::den;

    unordered_set<std::string> test_set;
    if (rank == 0) {
        ifstream test_data("./input/Rockyou-singleLined-full.txt");
        int test_count = 0;
        string pw;
        while (test_data >> pw) {
            test_count++;
            test_set.insert(pw);
            if (test_count >= 1000000) break;
        }
    }

    int cracked = 0;
    q.init();

    int curr_num = 0;
    auto start = system_clock::now();
    int history = 0;
    bool local_break = false;

    while (!q.priority.empty() && !PriorityQueue_mpi::global_terminate) {
        // 广播全局终止标志
        MPI_Bcast(&PriorityQueue_mpi::global_terminate, 1, MPI_C_BOOL, 0, MPI_COMM_WORLD);
        if (PriorityQueue_mpi::global_terminate) break;

        // 所有进程执行相同的PopNext操作
        q.PopNext();
        if (q.should_terminate) {
            // 如果队列为空，设置全局终止
            if (rank == 0) {
                PriorityQueue_mpi::global_terminate = true;
            }
            continue;
        }

        if (!q.priority.empty()) {
            PT next_pt = q.priority.front();
            q.Generate(next_pt);
        }
        
        // 只有rank0进行统计和终止检查
        if (rank == 0) {
            if (q.total_guesses - curr_num >= 100000) {
                cout << "Guesses generated: " << history + q.total_guesses << endl;
                curr_num = q.total_guesses;

                int generate_n = 1e7;
                if (history + q.total_guesses > generate_n) {
                    auto end = system_clock::now();
                    auto duration = duration_cast<microseconds>(end - start);
                    time_guess = double(duration.count()) * microseconds::period::num / microseconds::period::den;
                    cout << "Guess time:" << time_guess - time_hash << "seconds" << endl;
                    cout << "Hash time:" << time_hash << "seconds" << endl;
                    cout << "Train time:" << time_train << "seconds" << endl;
                    cout << "Cracked:" << cracked << endl;
                    PriorityQueue_mpi::global_terminate = true;
                }
            }

            if (curr_num > 1000000) {
                auto start_hash = system_clock::now();
                bit32 state[4];
                for (string pw : q.guesses) {
                    if (test_set.find(pw) != test_set.end()) {
                        cracked++;
                    }
                    MD5Hash(pw, state);
                }
                auto end_hash = system_clock::now();
                auto duration = duration_cast<microseconds>(end_hash - start_hash);
                time_hash += double(duration.count()) * microseconds::period::num / microseconds::period::den;

                history += curr_num;
                curr_num = 0;
                q.guesses.clear();
            }
        }
        
        // 广播可能的终止标志更新
        MPI_Bcast(&PriorityQueue_mpi::global_terminate, 1, MPI_C_BOOL, 0, MPI_COMM_WORLD);
    }
    
    // 最终终止确认
    if (rank == 0) {
        PriorityQueue_mpi::global_terminate = true;
    }
    MPI_Bcast(&PriorityQueue_mpi::global_terminate, 1, MPI_C_BOOL, 0, MPI_COMM_WORLD);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    run_process(rank, size);

    MPI_Finalize();
    return 0;
}