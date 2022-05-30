#include <iostream>
#include "thread"
#include "future"
#include "string"
using namespace std;

void take_value(std::promise<int>& p_val){
    int value;
    cout << "Insert a value: ";
    cin >> value;
    p_val.set_value(value);
}
void check_if_prime(std::future<int>& f_val, std::promise<bool>& p_res){
    int val = f_val.get();
    for(int i=2;i<=val/2; i++){
        if(val % i == 0) {
            p_res.set_value(false);
            return;
        }
    }
    p_res.set_value(true);
}
void give_answer(std::future<bool>& f_answer){
    bool answer = f_answer.get();
    string a = answer ? "prime":"not prime";
    cout << "The number is " << a << endl;
}
int main() {
    promise<int> p_val;
    promise<bool> p_res;
    future<int> f_val = p_val.get_future();
    future<bool> f_answer = p_res.get_future();

    thread th_take_value = thread(take_value, ref(p_val));
    thread th_check_if_prime = thread(check_if_prime, ref(f_val), ref(p_res));
    thread th_give_answer(give_answer, ref(f_answer));

    th_give_answer.join();
    th_take_value.join();
    th_check_if_prime.join();
    return 0;
}