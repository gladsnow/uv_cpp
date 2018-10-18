//#include <iostream>
//#include<functional>
//using namespace std;
//
//
//class A
//{
//public:
//typedef	std::function <void (int&)> Fun1;
//
//	A() {};
//	~A() {};
//	void setFun1Callback(const Fun1& cb)
//	{
//		fun1_callback_ = cb;
//	};
//
//	void print(void)
//	{
//		int a = 20;
//		fun1_callback_(a);
//	};
//
//private:
//	Fun1 fun1_callback_;
//
//};
//
//class B
//{
//public:
//	B() {
//		sum_add = 10;
//		a_.setFun1Callback(std::bind(&B::sum, this, std::placeholders::_1));
//	};
//	~B() {};
//
//	void display(void)
//	{
//		a_.print();
//	}
//
//	void sum(int& b) 
//	{
//		cout << "sum:" << b << endl;
//	}
//
//
//
//private:
//	A a_;
//	int sum_add;
//};
//
//int main()
//{
//	B b;
//	b.display();
//
//	return 0;
//}
//
