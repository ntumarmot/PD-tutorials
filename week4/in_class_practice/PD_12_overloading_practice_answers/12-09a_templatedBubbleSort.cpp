#include <iostream>
using namespace std;

template<typename T> 
void bubbleSort(const T unsorted[], T sorted[], T len);

int main()
{
  int unsorted[] = {1, 6, 3, 8, 7, 2, 4, 9, 0, 5};
  int len = sizeof(unsorted) / sizeof(unsorted[0]);
  int* sorted = new int[len]; 
   
  for(int i = 0; i < len; i++)
    cout << unsorted[i] << " ";
  cout << "\n";  
   
  bubbleSort<int>(unsorted, sorted, len);
  
  for(int i = 0; i < len; i++)
    cout << sorted[i] << " ";
  cout << "\n";  
  
  delete[] sorted; 
  
  return 0;
}

template<typename T> 
void bubbleSort(const T unsorted[], T sorted[], T len)
{
  for(int i = 0; i < len; i++)
    sorted[i] = unsorted[i];
    
  for(int i = len - 1; i > 0; i--)
  {
    for(int j = 0; j < i; j++)
    {
      if(sorted[j] > sorted[j + 1])
      {
        T temp = sorted[j];
        sorted[j] = sorted[j + 1];
        sorted[j + 1] = temp;
      }
    }
  }  
}

