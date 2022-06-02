#include <iostream>
#include "thread"
#include "chrono"
#include "queue"
#define NSLOTS 5
using namespace std;
std::mutex m;
std::queue<thread::id> parking;
int cars = 0;
std::mutex m_duration_simulation;
std::mutex m_queue;
std::condition_variable cv_duration;
int duration_simulation = 50;
int generate_random(int min, int max){
    return min + rand() % ((max + 1) - min);
}
void search_and_park(){
    int t1 = generate_random(1, 3);
    if(duration_simulation <= 0)
        return;
    std::this_thread::sleep_for (std::chrono::seconds(t1));
    std::unique_lock lock_m_duration_simulation{m_duration_simulation};
    duration_simulation -= t1;
    cout << duration_simulation << endl;
    if(duration_simulation <= 0){
        return; // simulation finished
    }
    lock_m_duration_simulation.unlock();
    // place a thread in the queue if not full
    std::unique_lock lock_m_queue{m_queue};
    if(cars < NSLOTS){
        parking.push(std::this_thread::get_id());
        cars++;
        cout << "Push thread " << std::this_thread::get_id() << " in the parking" << endl;
    }

    //cout << "Terminate thread: " << std::this_thread::get_id() << endl;
}

void leave_parking(){
    int t2 = generate_random(4, 7);
    if(duration_simulation <= 0)
        return;
    std::this_thread::sleep_for (std::chrono::seconds(t2));
    std::unique_lock lock_m_duration_simulation{m_duration_simulation};
    duration_simulation -= t2;
    cout << duration_simulation << endl;
    if(duration_simulation <= 0){
        return; // simulation finished
    }
    lock_m_duration_simulation.unlock();
    std::unique_lock lock_m_queue{m_queue};
    if(NSLOTS-cars > 0){
        parking.pop();
        cars--;
        cout << "Pop thread " << std::this_thread::get_id() << " from the parking" << endl;
    }
}

int main() {
    srand(time(NULL));
    std::vector<std::thread> threadPool;
    int i = 1000;
    while(i > 0){
        //cout << "Create" << endl;
        threadPool.emplace_back(std::thread(search_and_park));
        threadPool.emplace_back(std::thread(leave_parking));
        i--;
    }

    for(auto& t:threadPool){
        t.join();
    }
    cout << "Final queue of size: " << parking.size() << endl;
    while(!parking.empty()){
        cout << parking.front() << endl;
        parking.pop();
    }
    return 0;
}