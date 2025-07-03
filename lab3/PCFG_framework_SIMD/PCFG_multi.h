#include "PCFG.h"
#include <pthread.h>
#include <semaphore.h>
#include <queue>
#include <unistd.h>
#include <omp.h>
#define NUM_THREADS 4 // 定义线程数
using namespace std;

//---------------pthread实现的部分------------------



struct Task { // 用于传递给线程的数据结构
    int start; // 线程处理的起始索引
    int end; // 线程处理的结束索引
    segment* a;
    string prefix;  // 前缀字符串
};

struct ThreadData { // 用于存储线程共享数据
    bool active = false; // 线程是否处于活动状态
    bool terminate = false; // 线程是否需要终止
    int thread_id; // 线程ID
    PriorityQueue* queue; // 指向PriorityQueue的指针
};


// 优先队列(pthread实现)
class PriorityQueue_pthread : public PriorityQueue
{
public:
// 添加静态线程相关成员
    static pthread_t threads[NUM_THREADS];
    static bool threads_initialized;
    static sem_t task_sem; // 信号量，用于线程间通信
    static sem_t completion_sem;
    static pthread_mutex_t guesses_mutex; // 互斥锁，用于保护共享数据
    static pthread_mutex_t queue_mutex; // 互斥锁，用于保护共享数据
    static const int box = 5000; //定义每个线程的工作量
    static ThreadData shared_data[NUM_THREADS]; // 共享数据数组
    static queue<Task> task_queue; // 任务队列
    static int active_threads;

    void init();   // 覆写父类函数，加入初始化线程
    static void* thread_func(void* arg); // 线程函数
    void InitializeThreads();   //初始化线程
    void Generate(PT pt);   //覆写父类函数
    //void CleanupThreads();    // 在程序退出时调用
};

//---------------openmp实现的部分------------------

// 优先队列（OpenMP实现）
class PriorityQueue_openmp : public PriorityQueue
{
public:
    void Generate(PT pt); // 覆写父类函数
};


