/*
 * endpoint.cc
 *
 *  Created on: 2017年6月2日
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

		/*创建一个新的ZMQ环境上下文*/
		context_ = zmq_ctx_new();
		/*设置环境上下文属性可以连接的socket最大值*/
		zmq_ctx_set(context_, ZMQ_MAX_SOCKETS, 65536);
		std::string address = "tcp://*:" + std::to_string(current_.port);
		/*设置接收socket连接*/
		receiver_ = zmq_socket(context_, ZMQ_ROUTER);
		zmq_bind(receiver_, address.c_str());
		/*连接到调度节点*/
		Connect(scheduler_);
		// start receiver
		receiver_thread_ = std::unique_ptr <std::thread> (new std::thread(&Endpoint::Receiving, this));
		if (!is_scheduler_)
		{
			zmq_send (senders_[SchedulerID], "hello", 5, 0);
			cout<<"hello"<<endl;
			message msg;
			msg.sender=current_.id;
			msg.receiver=scheduler_.id;
			msg.timestamp=++timestamp;
			msg.request=true;
			msg.cmd=message::ADD_NODE;
			msg.push=false;
			//message.node=current_;//把单个节点信息扩展到多个
			msg.node.push_back(current_);
			//message.data_type.push_back();
			Send(msg);
		}
	}

	void Endpoint::Serialize(message& msg, char** meta_buf, int* meta_size){
		/*利用protobuf实现消息的序列化*/
		Meta meta;
		meta.set_cmd(msg.cmd);
		meta.set_sender(msg.sender);
		meta.set_receiver(msg.receiver);
		meta.set_timestamp(msg.timestamp);
		meta.set_request(msg.request);
		meta.set_push(msg.push);

		for (auto d : msg.data_type) meta.add_data_type(d);
  		for (auto n : msg.node)
		{
			auto n_ = meta.add_node(); 
			n_->set_role(n.role);
			n_->set_id(n.id);
			n_->set_hostname(n.hostname);
			n_->set_port(n.port);
			n_->set_is_recovery(n.is_recovery);
		}

		/*转为字符串*/
		*meta_size=meta.ByteSize();
		*meta_buf=new char[*meta_size+1];
	}

	void Endpoint::DeSerialize(){
		/*利用protobuf实现消息的反序列化*/
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
		/*如果已经有了socket连接，断开*/
		if (it != senders_.end()) {
			zmq_close(it->second);
		}
		/*以ZMQ_DEALER请求应答方式建立一个socket*/
		void *sender = zmq_socket(context_, ZMQ_DEALER);
		if (current_.id != Node::EmptyID) {
			std::string my_id = "ps" + std::to_string(current_.id);
			/*zmq_setsockopt设置socket属性，语义为设置身份ID*/
			zmq_setsockopt(sender, ZMQ_IDENTITY, my_id.data(), my_id.size());
		}
		// connect
		std::string addr = "tcp://" + node.hostname + ":"
				+ std::to_string(node.port);
		/*将socket连接到节点endpoint上*/
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
