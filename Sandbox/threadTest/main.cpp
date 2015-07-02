#include "thread.h"
#include <iostream>
#include <vector>
using namespace std;
int main()
{


	MyThread thread1("A"), thread2("B"), thread3("C");
	
	thread1.start();
	thread2.start();
	thread3.start();
	
	vector<joystickData> buf,buf1;

	int i;
	std::cin >> i;
	return 0;
}
