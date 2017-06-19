#include "params.h"
#include <iostream>
using namespace std;
using namespace ps;

void StartServer(){
	if(!IsServer())
		return;
	//auto server=new Server<float>(0);
}

void TrainWorker(){
	if(!IsWorker())
		return;
	//auto worker=new Worker<float>(0);
}

int main(){
	/*Start the parameter server*/
	Start();
	StartServer();
	TrainWorker();
	Stop();
	return 0;
}