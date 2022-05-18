#include "StackClass.h"
#include <iosfwd>
#include <iostream>
#include <exception>

template<typename T>
StackClass<T>::StackClass() {
    this->_size = 0;
    this->_dataContainer = new T;
}

template<typename T>
void StackClass<T>::push(const T &val) {
    // Re-allocate the stack
    T * newT = new T[this->_size+1];

    // Copy the current values in dataContainer
    std::memcpy(newT, this->_dataContainer, this->_size * sizeof(T));

    // Insert(push) new element
    newT[this->_size] = val;

    // Assign the new data container, increment stack size
    std::memcpy(this->_dataContainer, newT, (this->_size+1) * sizeof(T));
    this->_size ++;

    // Free newT
    delete[] newT;
}



template<typename T>
T StackClass<T>::pop() {
    // Check if the stack is empty
    if(this->empty())
        throw StackEmptyException();

    // Take value to pop
    T val = this->_dataContainer[this->_size - 1];

    // Resize the stack
    this->_size --;

    return val;
}

template<typename T>
bool StackClass<T>::empty() const {
    return this->_size==0;
}

template<typename T>
void StackClass<T>::setEmpty() {
    delete[] this->_dataContainer;
    this->_dataContainer = new T;
    this->_size = 0;
}
template<typename T>
T* StackClass<T>::getDataContainer(){
    return this->_dataContainer;
}

template<typename T>
std::vector<T> StackClass<T>::getStackAsVector() {
    std::vector<T> dataVector;

    int i;
    for(i=0; i<this->_size; i++){
        dataVector.push_back(this->_dataContainer[i]);
    }

    return dataVector;
}

template<typename T>
int StackClass<T>::getSize() const {
    return this->_size;
}

template<typename T>
StackClass<T>::~StackClass() {
    std::cout << "--Object Destructor--" << std::endl;
}

//copy constructor
template<typename T>
StackClass<T>::StackClass(const StackClass<T> & copyStack) {
    _size  = copyStack._size;
    _dataContainer = new T[copyStack._size];
    _dataContainer = copyStack._dataContainer;
}

//copy operator (copy assignement)
template<typename T>
StackClass<T>& StackClass<T>::operator=(const StackClass<T> & copyStack) {
    this->_size = copyStack._size;
    this->_dataContainer = new T[copyStack._size];
    this->_dataContainer = copyStack._dataContainer;
    return *this;
}

//move constructor
template<typename T>
StackClass<T>::StackClass(StackClass<T> && other) noexcept {
    _size = other._size;
    _dataContainer = new T[other._size];
    _dataContainer = other._dataContainer;
    other._dataContainer = nullptr; // if you do delete[]: _dataContainer is also deleted
    other._size = 0;
}

//move operator (=)
template<typename T>
StackClass<T>& StackClass<T>::operator=(StackClass<T> &&other) noexcept {
    this->_size = other._size;
    this->_dataContainer = new T[other._size];
    this->_dataContainer = other._dataContainer;
    return *this;
}

template<typename T>
void StackClass<T>::reverse() {
    int i;
    int size = this->_size;
    auto swap = [](int& a, int& b) -> void {
        int t = b;
        b = a;
        a = t;
    };
    for(i=0; i<size/2; i++){
        swap(this->_dataContainer[i], this->_dataContainer[size-i-1]);
    }
}

template<typename T>
StackClass<T> StackClass<T>::operator+(const StackClass<T> &toAdd) { // also needed overloading of move operator (=)
    StackClass<T> output;
    int i,j;
    output._size = this->_size + toAdd._size;
    output._dataContainer = new T[output._size];
    for(i=0,j=0; i<output._size; i++){
        if(i<this->_size)
            output._dataContainer[i] = this->_dataContainer[i];
        else
            output._dataContainer[i] = toAdd._dataContainer[j++];
    }
    return output;
}

template<typename T>
std::ostream & operator<<(std::ostream &os, const StackClass<T> &stack) {
    int i;
    for(i=0; i<stack._size; i++){
        os << stack._dataContainer[i] << std::endl;
    }
    return os;
}
