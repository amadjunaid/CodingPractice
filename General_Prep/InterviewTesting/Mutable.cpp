#include <iostream>
#include <memory>
#include <vector>
#include <string>

using std::cout;
using std::string;
class Mammal
{
protected:
	 string name_;
	 mutable int size;
public:
	Mammal()
	{
		name_ = "nameless";
	}
	Mammal(const string& name)
	{
		name_ = name;
	}

	void eat()
	{
		cout << "This Mammal " << name_ << " is eating" << std::endl;
	}

	void foo() const
	{
		size = 10;
	}
};

class Tiger : public virtual Mammal
{
public:
	void groom()
	{
		cout << "This Mammal " << name_ << " is T_groomed" << std::endl;
	}
};


void F(int A[], int B[], int n)
{
	for (int i = 0; i<n;i++)
	{
		auto res = [](int A[],int n, int i)->int {
			int result = 1;
			for (int j = 0; j < n; j++)
			{
				if (i != j)
					result *= A[j];
			}
			return result;
		};

		B[i] = res(A,n,i);
		
	}

}

int main()
{
	int A[] = { 2, 1 , 5 , 9 };
	int B[] = { 0,0,0,0 };

	F(A, B, 4);
	   
}