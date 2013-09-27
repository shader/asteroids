#pragma once
#include <list>
#include <functional>
using namespace std;

template<typename Args>
class Event {
	list<function<void(Args)>> handlers;
public:
	void operator+=(function<void(Args)> handler) {
		handlers.push_back( move(handler) );
	};

	void operator()(Args args) {
        for(auto& handler : handlers) {
			handler(args);
		}
	};
};