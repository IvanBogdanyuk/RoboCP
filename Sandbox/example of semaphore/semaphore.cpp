#include <iostream>
#include <string>
#include <queue>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <queue>



#include <boost/interprocess/sync/interprocess_semaphore.hpp>


boost::interprocess::interprocess_semaphore vacant(3);  
boost::interprocess::interprocess_semaphore use(0); 

using namespace std;
string line, first;
queue <string> q;


void read_str(){
	while(true){
	vacant.wait();
	cout << "write line" << endl;
	cin >> line;
	q.push(line);
	use.post();
	}
}

void write_str(){
	while (true){
	use.wait();
	first=q.front();
	q.pop();
	cout << "You write: "<< first << endl;	
	vacant.post();
	}
}

int main() {
    boost::thread_group tgroup;
	

	tgroup.create_thread ( boost::bind (&read_str) );
	tgroup.create_thread ( boost::bind (&write_str) );

	tgroup.join_all ();
	system("pause"); 
    return 0;
}