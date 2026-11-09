#include<iostream>
using namespace std;

// Function to swap two integers using references
void swap(int& a, int& b){
    int temp = a;
    a = b;
    b = temp;
}

// Function to swap two integers using pointers
void swap_using_ptr(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main()
{
    int a, b;
    cin >> a >> b;
    cout << "Original: " << a << " " << b << endl;
    swap_using_ptr(&a, &b);
    cout << "After swap: " << a << " " << b << endl;
}