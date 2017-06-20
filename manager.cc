/*
 * manager.cc
 *
 *  Created on: 2017��6��2��
 *      Author: zjbpoping
 */
#include "manager.h"
#include "environment.h"
#include "state.h"
#include <unistd.h>
#include <cstdlib>
#include <thread>
#include <chrono>

namespace ps{
	/*��ȡ����������ʼ��Manager����*/
	Manager::Manager(){

		ep_=new Endpoint();
		//const char* temp = NULL;
		//temp=Environment::Get()->find("NUM_WORKER");
		//num_workers_=atoi(temp);
		//temp=Environment::Get()->find("NUM_SERVER");
		//num_servers_ =atoi(temp);
		std::string role=Environment::Get()->find("ROLE");
		is_worker_ = (role=="worker"||role=="WORKER");
		is_server_ = (role=="server"||role=="SERVER");
		is_scheduler_ = (role=="scheduler"||role=="SCHEDULER");

	}

	void Manager::Start(){
		ep_->Start();
		start_time_ = time(NULL);
	}

	void Manager::Stop(){
		ep_->Stop();
	}

	void Manager::SetWorkerGroup(int id){
		for (int g : {id, WorkerGroupID, WorkerGroupID + ServerGroupID,
	        WorkerGroupID + SchedulerID,
	        WorkerGroupID + ServerGroupID + SchedulerID}) {
      		node_ids_[g].push_back(id);
    	}
	}

	void Manager::SetServerGroup(int id){
		for (int g : {id, ServerGroupID, WorkerGroupID + ServerGroupID,
	        ServerGroupID + SchedulerID,
	        WorkerGroupID + ServerGroupID + SchedulerID}) {
      		node_ids_[g].push_back(id);
    	}
	}

	Customer* Manager::GetCustomer(int id,int timeout) const{
		Customer* obj = nullptr;
		//cout<<"customers: "<<customers_.size()<<endl;
		for (int i = 0; i < timeout*1000+1; ++i) {
		    std::lock_guard<std::mutex> lk(mu_);
		    const auto it = customers_.find(id);
		    if (it != customers_.end()) {
		    	obj = it->second;
		        break;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		return obj;
	}

	void Manager::AddCustomer(Customer* customer){
		std::lock_guard<std::mutex> lk(mu_);
		int id=customer->Getid();
		customers_[id]=customer;
	}

	void Manager::RemoveCustomer(Customer* customer){
		std::lock_guard<std::mutex> lk(mu_);
		int id=customer->Getid();
		customers_.erase(id);
	}
}
