#include "params.h"
#include <iostream>
#include <unistd.h>
using namespace std;
using namespace ps;

template<typename Val>
class ServerHandle{
public:
	void operator()(KVMeta& req_meta,
			const KVPairs<Val>& req_data,
			Server<Val>* server){
		size_t n = req_data.keys.size();
        KVPairs<Val> res;
        if (req_meta.push) { //收到的是push请求
            //CHECK_EQ(n, req_data.vals.size());
        } else {            //收到的是pull请求
            res.keys = req_data.keys;
            res.vals.resize(n);
        }
        for (size_t i = 0;i < n; ++i) {
            Key key = req_data.keys[i];
            if (req_meta.push) {    //push请求
                store[key] += req_data.vals[i]; //此处的操作是将相同key的value相加
            } else {                    //pull请求
                res.vals[i] = store[key];
            }
        }
        server->Response(req_meta, res);
	}
private:
	std::unordered_map<Key, Val> store;
};

void StartServer(){
	if(!IsServer())
		return;
	Server<float> server(0);
	server.set_request_handle(ServerHandle<float>());
	sleep(1);
	while(true){
		
	}
}

void TrainWorker(){
	if(!IsWorker())
		return;
	Worker<float> worker(0);
	sleep(1);
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
	while(true){

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