/*
 * manager.h
 *
 *  Created on: 2017年6月2日
 *      Author: zjbpoping
 */
#ifndef MANAGER_H_
#define MANAGER_H_

#include "proc_commu.h"
#include "endpoint.h"
#include "state.h"
#include <unordered_map>
#include <mutex>

namespace ps{
	class Customer;
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
		// void SetWorkers(int num){num_workers_=num;}
		// void SetServers(int num){num_servers_=num;}
		
		/*判断当前节点类型*/
		bool IsWorker(){return is_worker_;}
		bool IsServer(){return is_server_;}
		bool IsScheduler(){return is_scheduler_;}
		/*启动节点管理器，同时会启动负责通信的endpoint*/
		void Start();
		/*终止节点管理器*/
		void Stop();

		int GetServerID(int num){
			return num*2+8;
		}

		int GetWorkerID(int num){
			return num*2+9;
		}

		void SetWorkerGroup(int id){
			for (int g : {id, WorkerGroupID, WorkerGroupID + ServerGroupID,
	            WorkerGroupID + SchedulerID,
	            WorkerGroupID + ServerGroupID + SchedulerID}) {
      			node_ids_[g].push_back(id);
    		}
		}

		void SetServerGroup(int id){
			for (int g : {id, ServerGroupID, WorkerGroupID + ServerGroupID,
	            ServerGroupID + SchedulerID,
	            WorkerGroupID + ServerGroupID + SchedulerID}) {
      			node_ids_[g].push_back(id);
    		}
		}

		Customer* GetCustomer(int id,int timeout=0) const;
		void AddCustomer(Customer* customer);
		void RemoveCustomer(Customer* customer);
		const std::vector<int>& GetNodeIDs(int node_id) const{
			const auto it=node_ids_.find(node_id);
			return it->second;
		}

	private:

		Manager();
		~Manager(){delete ep_;}
		/*该节点的通信断端点，也是一个节点只有一个*/
		Endpoint* ep_;
		int num_workers_=0,num_servers_=0;
		bool is_worker_,is_server_,is_scheduler_;
		time_t start_time_;
		std::unordered_map<int,Customer*> customers_;
		//还需要添加node_ids的初始化过程，思路可以是每次计算好num_workers_之后调用
		std::unordered_map<int,std::vector<int>> node_ids_;
		mutable std::mutex mu_;
	};


}
#endif
