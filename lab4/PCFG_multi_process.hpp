// MULTIPROCESS 部分修改
#include "PCFG.h"
#include <mpi.h>
#include <cstring>
#define MAX_STR_LEN 256

using namespace std;

class PriorityQueue_mpi : public PriorityQueue {
public:
    void Generate(PT pt) override;
    void PopNext() override;
    
    // 添加终止标志
    bool should_terminate = false;
    
    // 添加全局终止标志
    static bool global_terminate;
};

bool PriorityQueue_mpi::global_terminate = false;

void PriorityQueue_mpi::Generate(PT pt) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // 检查全局终止标志
    if (global_terminate) {
        should_terminate = true;
        return;
    }

    segment* target_seg = nullptr;
    int total_values = 0;
    string prefix;

    if (pt.content.size() == 1) {
        target_seg = &pt.content[0];
        total_values = pt.max_indices[0];
    } else {
        int last_idx = pt.content.size() - 1;
        target_seg = &pt.content[last_idx];
        total_values = pt.max_indices[last_idx];

        for (int i = 0; i < pt.curr_indices.size(); ++i) {
            segment& seg = pt.content[i];
            int idx = pt.curr_indices[i];
            if (seg.type == 1)
                prefix += m.letters[m.FindLetter(seg)].ordered_values[idx];
            else if (seg.type == 2)
                prefix += m.digits[m.FindDigit(seg)].ordered_values[idx];
            else if (seg.type == 3)
                prefix += m.symbols[m.FindSymbol(seg)].ordered_values[idx];
        }
    }

    segment* actual_seg = nullptr;
    switch (target_seg->type) {
        case 1: actual_seg = &m.letters[m.FindLetter(*target_seg)]; break;
        case 2: actual_seg = &m.digits[m.FindDigit(*target_seg)]; break;
        case 3: actual_seg = &m.symbols[m.FindSymbol(*target_seg)]; break;
    }

    // 确保所有进程都有工作可做
    if (total_values == 0) {
        if (rank == 0) {
            guesses.push_back(prefix);
            total_guesses++;
        }
        return;
    }

    int chunk_size = total_values / size;
    int remainder = total_values % size;
    int start = rank * chunk_size + min(rank, remainder);
    int end = start + chunk_size + (rank < remainder ? 1 : 0);
    if (end > actual_seg->ordered_values.size()) 
        end = actual_seg->ordered_values.size();

    vector<string> local_guesses;
    for (int i = start; i < end; ++i) {
        if (pt.content.size() == 1) {
            local_guesses.push_back(actual_seg->ordered_values[i]);
        } else {
            local_guesses.push_back(prefix + actual_seg->ordered_values[i]);
        }
    }

    // 使用非阻塞通信收集结果
    if (rank == 0) {
        // 先添加rank0自己的结果
        guesses.insert(guesses.end(), local_guesses.begin(), local_guesses.end());
        total_guesses += local_guesses.size();
        
        // 接收其他进程的结果
        MPI_Request* requests = new MPI_Request[size-1];
        vector<int> counts(size-1, 0);
        vector<vector<char>> buffers(size-1);

        // 非阻塞接收计数
        for (int proc = 1; proc < size; proc++) {
            MPI_Irecv(&counts[proc-1], 1, MPI_INT, proc, 0, MPI_COMM_WORLD, &requests[proc-1]);
        }
        
        // 等待所有计数接收完成
        MPI_Waitall(size-1, requests, MPI_STATUSES_IGNORE);
        
        // 准备接收数据
        for (int proc = 1; proc < size; proc++) {
            int count = counts[proc-1];
            if (count > 0) {
                buffers[proc-1].resize(count * MAX_STR_LEN);
                MPI_Irecv(buffers[proc-1].data(), count * MAX_STR_LEN, MPI_CHAR, proc, 0, MPI_COMM_WORLD, &requests[proc-1]);
            }
        }
        
        // 等待数据接收完成
        for (int proc = 1; proc < size; proc++) {
            int count = counts[proc-1];
            if (count > 0) {
                MPI_Wait(&requests[proc-1], MPI_STATUS_IGNORE);
                
                // 处理接收到的数据
                char* buffer = buffers[proc-1].data();
                for (int i = 0; i < count; i++) {
                    char* str = buffer + i * MAX_STR_LEN;
                    guesses.emplace_back(str);
                    total_guesses++;
                }
            }
        }
        
        delete[] requests;
    } else {
        // 非rank0进程发送结果
        int count = local_guesses.size();
        MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        
        if (count > 0) {
            char* buffer = new char[count * MAX_STR_LEN];
            for (int i = 0; i < count; i++) {
                strncpy(buffer + i * MAX_STR_LEN, local_guesses[i].c_str(), MAX_STR_LEN-1);
                buffer[i * MAX_STR_LEN + MAX_STR_LEN-1] = '\0';
            }
            MPI_Send(buffer, count * MAX_STR_LEN, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            delete[] buffer;
        }
    }
}

void PriorityQueue_mpi::PopNext() {
    if (priority.empty() || global_terminate) {
        should_terminate = true;
        return;
    }

    // 所有进程执行相同的队列操作
    PT pt_to_process = priority.front();
    vector<PT> new_pts = pt_to_process.NewPTs();

    for (PT& pt : new_pts) {
        CalProb(pt);
        auto it = priority.begin();
        while (it != priority.end() && it->prob > pt.prob) ++it;
        priority.insert(it, pt);
    }

    priority.erase(priority.begin());
}
