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
		is_server_ = (role=="server"||role=="worker");
		is_scheduler_ = (role=="scheduler"||role=="SCHEDULER");

	}

	void Manager::Start(){
		ep_->Start();
		start_time_ = time(NULL);
	}

	void Manager::Stop(){
		ep_->Stop();
	}
}
