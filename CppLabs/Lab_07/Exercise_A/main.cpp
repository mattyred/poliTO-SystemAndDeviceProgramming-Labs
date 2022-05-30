#include "iostream"
#include "thread"
#include "vector"
#include "time.h"
#include "atomic"

using namespace std;
int var = 0;
std::mutex m;
std::condition_variable cv_add, cv_admin;
int generate_random(int min, int max);
void admin_f();
void adder_f();

void admin_f(){
    //this_thread::sleep_for(chrono::seconds(3));
    std::unique_lock lock_cv{m};
    var = 10;
    lock_cv.unlock();
    cout << "Now adder threads can work" << endl;
    cv_add.notify_all();
    lock_cv.lock();
    cv_admin.wait(lock_cv);
    if(var == 15)
        cout << "Notified that Var == 15" << endl;
    else
        cout << "Notified that threads are dead" << endl;
}

void adder_f(){
    std::unique_lock lock_cv{m};
    while(var==0){
        cv_add.wait(lock_cv);
    }
    if(var == 15)
        return;
    cout << "Adding random to var" << endl;
    var += generate_random(0,5);
    if(var == 15){
        cout << "Var == 15" << endl;
        cv_admin.notify_one();
    }
}
int generate_random(int min, int max){
    return min + rand() % ((max + 1) - min);
}
int main() {
    srand(time(NULL));
    std::thread t_admin(admin_f);
    std::vector<std::thread> vector_threads;
    for(int i=0;i<3;i++)
        vector_threads.emplace_back(adder_f);

    for(auto& t:vector_threads)
        t.join();
    cv_admin.notify_one(); // awake admin threads that all adders threads have finished
    t_admin.join();

    std::cout << "Final value of var: " << var << std::endl;
    return 0;
}