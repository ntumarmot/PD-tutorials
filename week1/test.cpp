#include <iostream>
#include <vector>
using namespace std;

int main(){
    vector<int> v(5, 1000);
    for(int i=0; i<v.size(); i++){
        cout << v[i] << " ";
    }
    return 0;
}

