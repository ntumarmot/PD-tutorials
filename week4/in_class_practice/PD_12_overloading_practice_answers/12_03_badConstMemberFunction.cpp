// ===========================================================================
// Operator Overloading 
// ===========================================================================

#include <iostream>
#include <cstdlib>
using namespace std;

// class definition of MyVector
class MyVector
{
friend const MyVector operator+(const MyVector& v, double d);
private:
  int n; 
  double* m; 
public:
  MyVector();
  MyVector(int n, double m[]);  
  MyVector(const MyVector& v);
  ~MyVector();
  void print() const; 
  
  bool operator==(const MyVector& v) const;
  bool operator!=(const MyVector& v) const;
  bool operator<(const MyVector& v) const;

  // =============================
  // WATCH THIS!!
  double& operator[](int i) const;
  // =============================  

  const MyVector& operator=(const MyVector& v);
  const MyVector& operator+=(const MyVector& v);
};
// end of class definition of MyVector

// global function headers
const MyVector operator+(const MyVector& v, double d);
const MyVector operator+(double d, const MyVector& v);
const MyVector operator+(const MyVector& v1, const MyVector& v2);





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
bool MyVector::operator!=(const MyVector& v) const
{
  return !(*this == v);
}
bool MyVector::operator<(const MyVector& v) const
{
  if(this->n != v.n)
    return false;
  else
  {
  	for(int i = 0; i < n; i++)
  	{
  	  if(this->m[i] >= v.m[i])
  	    return false;
  	}
  }	
  return true;
}









// ==========================================
// WATCH THIS!!
double& MyVector::operator[](int i) const
{
  if(i < 0 || i >= n)
    exit(1);
  return m[i];
}
// ==========================================












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
}const MyVector& MyVector::operator+=(const MyVector& v)
{
  if(this->n == v.n)
  {
    for(int i = 0; i < n; i++)
      this->m[i] += v.m[i]; 
  }
  return *this;
}
// end of MyVector's overloaded operators

// global functions
const MyVector operator+(const MyVector& v, double d)
{
  MyVector sum(v);
  for(int i = 0; i < v.n; i++)
    sum[i] += d;
  return sum;
}
const MyVector operator+(double d, const MyVector& v)
{
  return v + d;
}
const MyVector operator+(const MyVector& v1, const MyVector& v2)
{
  MyVector sum(v1); 
  sum += v2;
  return sum;
}
// end of global functions
// ===========================================================================






int main()
{
  double d = 1.23;
  double m[3] = {d, d, d + 0.00000001};
  const MyVector v(3, m); // v is a constant object 
  v.print();
  
  cout << v[0] << "\n";
  v[0] = 1;
  cout << v[0] << "\n"; // See whether the constant object v is modified! 
  
  return 0;
}
