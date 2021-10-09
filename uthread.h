//#ifndef _UTHREAD_H_
//#define _UTHREAD_H_


#include <iostream>
#include <ucontext.h>
#include <unistd.h>
#include <vector>

#define DEFAULT_STACK_SIZE (1024*128)
#define MAX_THREAD_SIZE 128

enum ThreadState{FREE, RUNNABLE, RUNNING, SUSPEND};
                //空闲，  就绪，   正在运行， 挂起 四种状态

using namespace std;
typedef void (* Fun)(void * arg);

typedef struct uthread_t {
    ucontext_t ctx;                 //保存协程的context
    Fun func;                       //协程执行的用户函数
    void * arg;                     //函数参数
    enum ThreadState state;         //协程的运行状态
    char stack[DEFAULT_STACK_SIZE]; //协程的栈

} uthread_t;

typedef std::vector<uthread_t> Thread_vector;

typedef struct schedule_t {     //调度器
    ucontext_t main;        //保存主函数的context
    int running_thread;     //正在运行的thread序号，若无，则为-1；
    Thread_vector threads;  

    int max_index;      //当前含有的协程

    schedule_t():running_thread(-1), max_index(0){
        threads.resize(MAX_THREAD_SIZE);
        for(auto th : threads) {
            th.state = FREE;
        }
    };

    ~schedule_t() {
        threads.clear();
    }

} schedule_t;

//创建协程
int uthread_create(schedule_t &schedule, Fun func, void * arg); 

//挂起正在运行的协程，切换到主函数
void uthread_yield(schedule_t &schedule); 

//恢复序号id的协程运行
void uthread_resume(schedule_t &schedule, int id); 

//判断所有协程是否执行完毕，是则返回1，否则返回0（挂起状态不算执行完毕）
int schedule_finished(const schedule_t & schedule);

//运行协程
void thread_body(schedule_t& st);
//#endif