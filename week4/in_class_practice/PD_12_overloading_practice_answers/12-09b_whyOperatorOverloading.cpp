#include <iostream>
#include <cstdlib> 
using namespace std;

// class definition of MyVector
class MyVector
{
private:
  static double equalityGap;
  int n; 
  double* m; 
public:
  MyVector();
  MyVector(int n, double m[]);  
  MyVector(const MyVector& v);
  const MyVector& operator=(const MyVector& v);
  ~MyVector();
  void print() const; 
  
  bool operator==(const MyVector& v) const;
  bool operator>(const MyVector& v) const;
// end of class definition of MyVector
};




double MyVector::equalityGap = 0.00001;




// MyVector's instance functions
MyVector::MyVector(): n(0), m(NULL) 
{
}
MyVector::MyVector(int n, double m[])
{
  this->n = n;
  this->m = new double[n];
  for(int i = 0; i < n; i++)
    this->m[i] = m[i];
}
MyVector::MyVector(const MyVector& v)
{
  this->n = v.n;
  this->m = new double[n];
  for(int i = 0; i < n; i++)
    this->m[i] = v.m[i];	
}
const MyVector& MyVector::operator=(const MyVector& v)
{
  if(this != &v)
  {
    if(this->n != v.n)
    {
      delete [] this->m;
      this->n = v.n;
      this->m = new double[this->n];
    }
    for(int i = 0; i < n; i++)
      this->m[i] = v.m[i];
  }  
  return *this;
}
MyVector::~MyVector() 
{ 
  delete [] m; 
}
void MyVector::print() const 
{
  cout << "(";
  for(int i = 0; i < n - 1; i++)
    cout << m[i] << ", ";
  cout << m[n-1] << ")\n";
}
// end of MyVector's instance functions

// MyVector's overloaded operators
bool MyVector::operator==(const MyVector& v) const
{
  if(this->n != v.n)
    return false;
  else
  {
    for(int i = 0; i < n; i++)
    {
      if(this->m[i] != v.m[i])
      return false;
    }
  }	
  return true;
}

bool MyVector::operator>(const MyVector& v) const
{
  // assuming their dimensions are the same 
  int sum = 0, sumV = 0;
  for(int i = 0; i < this->n; i++)
  {
    sum += this->m[i];
    sumV += v.m[i];
  }
  if(sum > sumV)
    return true; 
  else
    return false; 
}
















template<typename T> 
void bubbleSort(const T unsorted[], T sorted[], int len);

int main()
{
  // Creating three unsorted MyVector objects
  double d1[] = {9, 2, 3};
  double d2[] = {2, 7, 3};
  double d3[] = {1, 3, 3};
   
  MyVector unsorted[] =  
  {
    MyVector(3, d1), MyVector(3, d2), MyVector(3, d3) 
  }; 
   
  // Creating a place to store the sorted MyVector objects
  int len = sizeof(unsorted) / sizeof(unsorted[0]);
  MyVector* sorted = new MyVector[len]; 
  
  // Using the existing function to sort these MyVector objects
  for(int i = 0; i < len; i++)
    unsorted[i].print();
  cout << "\n";  

  bubbleSort<MyVector>(unsorted, sorted, len);
  
  for(int i = 0; i < len; i++)
    sorted[i].print();
  cout << "\n";  
  
  // Releasing the memory space
  delete[] sorted; 
  
  return 0;
}

template<typename T> 
void bubbleSort(const T unsorted[], T sorted[], int len)
{
  for(int i = 0; i < len; i++)
    sorted[i] = unsorted[i];
    
  for(int i = len - 1; i > 0; i--)
  {
    for(int j = 0; j < i; j++)
    {
      if(sorted[j] > sorted[j + 1]) // If someone implemented this function, and you want to use it, 
	                                // you must overload ">" for your class.
	                                // You just cannot name it "isGreaterThan".  
      {
        T temp = sorted[j];
        sorted[j] = sorted[j + 1];
        sorted[j + 1] = temp;
      }
    }
  }  
}

