/*
 * server.h
 *
 *  Created on: 2017年6月15日
 *      Author: zjbpoping
 */
#ifndef SERVER_H_
#define SERVER_H_

#include "proc.h"
#include "value.h"
#include <utility>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>

namespace ps{

	template <typename Val>
	class Server:public Proc{
	public:
		explicit Server(int proc_id):Proc(){
			using namespace std::placeholders;
			customer=new Customer(proc_id,std::bind(&Server<Val>::Process,this,_1));
		}
		virtual ~Server(){
			delete customer;
			customer=nullptr;
		}
		using ReqHandle=std::function<void(KVMeta& req_meta,
			const KVPairs<Val>& req_data,
			Server* server)>;

		void set_request_handle(const ReqHandle& handle){
			request_handle_=handle;
		}
		/*对Push/Pull消息的回复，传入的是请求者的KVMeta*/
		void Response(const KVMeta& req,const KVPairs<Val>& res=KVPairs<Val>());

	private:
		/*customer中消息处理的handle*/
		void Process(message& msg);
		/*消息请求handle, 消息处理process中需要*/
		ReqHandle request_handle_;
	};

	template <typename Val>
	void Server<Val>::Response(const KVMeta& req,const KVPairs<Val>& res){
		///
	}

	template <typename Val>
	void Server<Val>::Process(message& msg){
		KVMeta meta;
		meta.cmd=0;
		meta.push=msg.push;
		meta.sender=msg.sender;
		meta.timestamp=msg.timestamp;
		KVPairs<Val> data;
		int n=msg.data.size();
		if(n){
			std::string keys=msg.data[0];
			std::string vals=msg.data[1];
			if(n>2)
			{
				std::string lens=msg.data[2];
			}
		}
		request_handle_(meta,data,this);
	}

}

#endif 