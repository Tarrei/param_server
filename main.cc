#include "params.h"
#include <iostream>
#include <unistd.h>
using namespace std;
using namespace ps;

void StartServer(){
	if(!IsServer())
		return;
	// auto server=new Server<float>(0);
	// sleep(1000);
}

void TrainWorker(){
	if(!IsWorker())
		return;
	Worker<float> worker(0);
	sleep(1000);
	int num=10;
	vector<Key> keys(num);
	vector<float> vals(num);
	for(int i=0;i<num;i++)
	{
		keys[i]=i;
		vals[i]=i+10;
	}
	int repeat=1;
	vector<int> ts;
	for(int i=0;i<repeat;i++){
		ts.push_back(worker.Push(keys,vals));
	}
}

int main(){
	/*Start the parameter server*/
	Start();
	StartServer();
	TrainWorker();
	Stop();
	return 0;
}