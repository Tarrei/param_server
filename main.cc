#include "params.h"
#include <iostream>
using namespace std;
using namespace ps;

void TrainWorker(){
	if(!IsWorker())
		return;
	auto worker=new Worker<float>(0);
}
int main(){
	/*Start the parameter server*/
	Start();
	Stop();
	return 0;
}