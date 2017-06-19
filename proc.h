/*
 * proc.h
 *
 *  Created on: 2017年6月15日
 *      Author: zjbpoping
 */
#ifndef COMPUTE_H_
#define COMPUTE_H_
#include "node.h"
#include "message.h"
#include "manager.h"
#include <string>

namespace ps{
	class Proc{
	public:
		explicit Proc(int proc_id);
		virtual ~Proc(){
			delete customer;
			customer=nullptr;
		}
		//virtual inline int Request();
		virtual inline void Wait(int timestamp){
			customer->WaitRequest(timestamp);
		};
		// virtual inline void Response();
		// //using Handle = std::function<void()>;
		// virtual inline void set_request_handle(const Handle& request){
		// 	request_handle_=request;
		// }
		// virtual inline void set_response_handle(const Handle& response){
		// 	response_handle_=response;
		// }
		virtual inline Customer* get_customer(){
			return customer;
		}
	protected:
		inline Proc():customer(nullptr){};
		// Handle request_handle_;
		// Handle response_handle_;
		Customer* customer;
	};
}

#endif