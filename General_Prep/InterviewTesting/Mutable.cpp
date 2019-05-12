#include <iostream>
#include <memory>
using std::cout;

class A {
public:
	A() { cout << "create A\n";; }
	virtual ~A() { cout << "destroy A\n"; }

};

class B : public A 
{
public:
	B() { cout << "create B\n";; }
	~B() { cout << "destroy B\n"; }
};

int main()
{
	A *a = new A();
	delete a;

	A *a_2 = new B();
	delete a_2;

	std::shared_ptr<A> u_a = std::shared_ptr<A>();

	std::shared_ptr<A> u_b = u_a;
}