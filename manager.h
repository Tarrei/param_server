/*
 * manager.h
 *
 *  Created on: 2017年6月2日
 *      Author: zjbpoping
 */
#ifndef MANAGER_H_
#define MANAGER_H_

#include "endpoint.h"

namespace ps{
	/*parameter Server节点管理器，思路：节点崩溃和添加可以通过Scheduler节点通信来确定*/
	class Manager{
	public:
		/*返回一个静态的Manager对象,一个节点只有这么一个*/
		static Manager* Get(){
			static Manager manager;
			return &manager;
		}
		/*返回endpoint指针*/
		Endpoint* GetEndpoint(){return ep_;	}
		int NumWorkers(){return num_workers_;}
		int NumServers(){return num_servers_;}
		void AddWorkers(){num_workers_++;}
		void AddServers(){num_servers_++;}
		void SetWorkers(int num){num_workers_=num;}
		void SetServers(int num){num_servers_=num;}
		/*判断当前节点类型*/
		bool IsWorker(){return is_worker_;}
		bool IsServer(){return is_server_;}
		bool IsScheduler(){return is_scheduler_;}
		/*启动节点管理器，同时会启动负责通信的endpoint*/
		void Start();
		/*终止节点管理器*/
		void Stop();

	private:

		Manager();
		~Manager(){delete ep_;}
		/*该节点的通信断端点，也是一个节点只有一个*/
		Endpoint* ep_;
		int num_workers_=0,num_servers_=0;
		bool is_worker_,is_server_,is_scheduler_;
		time_t start_time_;
	};


}
#endif
