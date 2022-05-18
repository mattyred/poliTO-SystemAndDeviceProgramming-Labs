#include <iostream>
#include "StackClass.h"
#include "StackClass.cpp"
#include <vector>

int main()
{
    StackClass<int> myStack;
    int val;

    myStack.push(2);
    myStack.push(3);
    myStack.push(7);
    std::cout << "Stack content:" << std::endl;
    std::cout << myStack;


    // TEST POP AND EXCEPTION
    /*
    for(int i=0; i<4; i++){
        try {
            val = myStack.pop();
            std::cout << "Pop value: " << val << std::endl;
        }
        catch(const StackEmptyException& exception){
            std::cout << exception.what() << std::endl;
        }
    }*/


    //TEST operator+
    StackClass<int> toAdd;
    toAdd.push(8);
    toAdd.push(9);
    toAdd.push(1);
    StackClass<int> concatenation;
    //concatenation = myStack + toAdd;
    concatenation = myStack + toAdd;
    std::cout << concatenation;


    //test GETASVECTOR
    std::vector<int> dataVector;
    dataVector = concatenation.getStackAsVector();
    std::cout << "v = { ";
    for (int n : dataVector) {
        std::cout << n << ", ";
    }
    std::cout << "}; \n";

    //TEST copy constructor and assignment operator
    StackClass<int> stack1 = myStack; //copy constructor
    stack1.push(3);
    stack1.push(8);
    StackClass<int> stack2;
    stack2 = stack1; // assignment operator
    std::cout << "STACK 1" << std::endl;
    std::cout << stack1;
    std::cout << "STACK 2" << std::endl;
    std::cout << stack2;


    // test REVERSE
    stack1.setEmpty();
    stack1.push(1);
    stack1.push(2);
    stack1.push(3);
    StackClass<int> reversedStack = stack1;
    reversedStack.reverse();
    std::cout << "Reversed stack:" << std::endl;
    std::cout << reversedStack;

    // test MOVE CONSTRUCTOR
    StackClass<int> movedStack = std::move(stack1);
    std::cout << "Stack built with move constructor:" << std::endl;
    std::cout << movedStack;

    // Random
    int * v1 = new int[10];
    for(int i=0;i<10;i++)
        v1[i] = i;
    int * v2 = new int[10];
    /*
    // Shallow copy of v1 into v2
    v2 = v1;
    v1[1] = 87;
    delete[] v1; // also v2 is de-allocated!
     */
    std::memcpy(v2, v1, 10*sizeof(int));
    v1[1] = 87; // this does not modify v2!
    delete[] v1;
    return 0;
}
