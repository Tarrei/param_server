/*
 * endpoint.cc
 *
 *  Created on: 2017��6��2��
 *      Author: zjbpoping
 */
#include "endpoint.h"
#include "environment.h"
#include "manager.h"
#include "state.h"
#include "message.pb.h"
#include <zmq.h>
#include <thread>
#include <stdio.h>
#include <chrono>
#include <iostream>
using namespace std;

namespace ps {

	void Endpoint::Start() {

		scheduler_.hostname = std::string(Environment::Get()->find("PS_ROOT_URI"));
		scheduler_.port = atoi(Environment::Get()->find("PS_ROOT_PORT"));
		scheduler_.role = Node::SCHEDULER;
		scheduler_.id = SchedulerID;
		is_scheduler_ = Manager::Get()->IsScheduler();
	

		if (is_scheduler_) {
			current_ = scheduler_;
		} else {
			auto role =
					is_scheduler_ ?
							Node::SCHEDULER :
							(Manager::Get()->IsWorker() ?
									Node::WORKER : Node::SERVER);
			const char* nhost = Environment::Get()->find("NODE_URI");
			std::string ip = std::string(nhost);
			const char* pstr = Environment::Get()->find("NODE_PORT");
			int port = atoi(pstr);
			current_.hostname = ip;
			current_.role = role;
			current_.port = port;
			// cannot determine my id now, the scheduler will assign it later
			// set it explicitly to make re-register within a same process possible
			current_.id = Node::EmptyID;
		}

		/*����һ���µ�ZMQ����������*/
		context_ = zmq_ctx_new();
		/*���û������������Կ������ӵ�socket���ֵ*/
		zmq_ctx_set(context_, ZMQ_MAX_SOCKETS, 65536);
		std::string address = "tcp://*:" + std::to_string(current_.port);
		/*���ý���socket����*/
		receiver_ = zmq_socket(context_, ZMQ_ROUTER);
		zmq_bind(receiver_, address.c_str());
		/*���ӵ����Ƚڵ�*/
		Connect(scheduler_);
		// start receiver
		receiver_thread_ = std::unique_ptr <std::thread> (new std::thread(&Endpoint::Receiving, this));
		if (!is_scheduler_)
		{
			zmq_send (senders_[SchedulerID], "hello", 5, 0);
			cout<<"hello"<<endl;
			message msg;
			message.sender=current_.id;
			message.receiver=scheduler_.id;
			message.timestamp=++timestamp;
			message.request=true;
			message.cmd=ADD_NODE;
			message.push=false;
			//message.node=current_;//�ѵ����ڵ���Ϣ��չ�����
			message.node.push_back(current_);
			//message.data_type.push_back();
			Send(msg);
		}
	}

	void Endpoint::Serialize(message& msg, char** meta_buf, int* meta_size){
		/*����protobufʵ����Ϣ�����л�*/
		meta meta��
	}

	void Endpoint::DeSerialize(){
		/*����protobufʵ����Ϣ�ķ����л�*/
	}

	void Endpoint::Send(message& msg){
		int id=msg.receiver;
		void *socket = senders_.find(id)->second;
		int meta_size;
		char* meta_buf;
		Serialize(msg,&meta_buf,&meta_size);
	}

	void Endpoint::Connect(const Node& node) {
		int id = node.id;
		auto it = senders_.find(id);
		/*����Ѿ�����socket���ӣ��Ͽ�*/
		if (it != senders_.end()) {
			zmq_close(it->second);
		}
		/*��ZMQ_DEALER����Ӧ��ʽ����һ��socket*/
		void *sender = zmq_socket(context_, ZMQ_DEALER);
		if (current_.id != Node::EmptyID) {
			std::string my_id = "ps" + std::to_string(current_.id);
			/*zmq_setsockopt����socket���ԣ�����Ϊ�������ID*/
			zmq_setsockopt(sender, ZMQ_IDENTITY, my_id.data(), my_id.size());
		}
		// connect
		std::string addr = "tcp://" + node.hostname + ":"
				+ std::to_string(node.port);
		/*��socket���ӵ��ڵ�endpoint��*/
		zmq_connect(sender, addr.c_str());
		senders_[id] = sender;
	}

	void Endpoint::Receiving() {
		int count=0;
		while(true)
		{
			char buffer[10];
			zmq_recv (receiver_, buffer, 10, 0);
			std::string test=std::string(buffer);
			if(test=="hello"){
				//
			}else{
				//
			}
		}
	}

	void Endpoint::Stop(){
		receiver_thread_->join();
		// if (!is_scheduler_) 
		// 	heartbeat_thread_->join();
	}
}
