/*
 * value.h
 *
 *  Created on: 2017年6月16日
 *      Author: zjbpoping
 */
#ifndef VALUE_H_
#define VALUE_H_

#include <vector>

using namespace std;

namespace ps{

	#if USE_KEY32
		using Key = uint32_t;
	#else
		using Key = uint64_t;
	#endif

	template <typename Val>
	struct KVPairs{
		vector<Key> keys;
		vector<Val> vals;
		vector<int> lens;
	};

	struct KVMeta{
		int cmd;
		bool push;
		int sender;
		int timestamp;
	};

	class Range {
	public:
	  	Range() : Range(0, 0) {}
	  	Range(uint64_t begin, uint64_t end) : begin_(begin), end_(end) { }

		uint64_t begin() const { return begin_; }
		uint64_t end() const { return end_; }
		uint64_t size() const { return end_ - begin_; }
	private:
	  	uint64_t begin_;
	  	uint64_t end_;
	};

}

#endif