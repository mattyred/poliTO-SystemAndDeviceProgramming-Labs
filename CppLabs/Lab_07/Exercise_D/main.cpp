#include <iostream>
#include "thread"
#include "chrono"
#include "vector"
using namespace std;
std::mutex m;
std::condition_variable cv;
float t_cur = 18.5;
float t_target = 18;
bool heating = false;

void targetTemp(int time){
    while(true){
        std::unique_lock ul{m};
        std::cout << "Insert target temperature: "; // Type a number and press enter
        std::cin >> t_target;
        if(t_target==-1){
            ul.unlock();
            break;
        }
        ul.unlock();
        std::this_thread::sleep_for (std::chrono::seconds(time)); // wait 5 seconds
    }
}
void currentTemp(int time){
    float t;
    while(true){
        std::unique_lock ul{m};
        if(t_target==-1){
            ul.unlock();
            break;
        }
        t = t_cur;
        heating ? t_cur+=0.3 : t_cur-=0.3;
        cout << "Current temperature from " << t << " to " << t_cur << endl;
        if(t_cur == t_target){
            t_target = -1;
            ul.unlock();
            break;
        }
        ul.unlock();
        std::this_thread::sleep_for (std::chrono::seconds(time)); // wait 5 seconds
    }
}

void admin(int time){
    while(true){
        std::unique_lock ul{m};
        if(t_target == -1){
            ul.unlock();
            break;
        }
        if(heating && t_cur > t_target + 0.2)
            heating = false;
        else if(!heating && t_cur <= t_target + 0.2)
            heating = true;
        ul.unlock();
        std::this_thread::sleep_for (std::chrono::seconds(time)); // wait 3 seconds
    }
}

int main() {
    std::vector<std::thread> threadPool;
    threadPool.emplace_back(std::thread(currentTemp, 5));
    threadPool.emplace_back(std::thread(targetTemp, 5));
    threadPool.emplace_back(std::thread(admin, 3));

    for(auto& t:threadPool)
        t.join();

    return 0;
}