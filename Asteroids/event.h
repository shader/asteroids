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

	void clear() {
		handlers.clear();
	};

	void operator()(Args args) {
        for(auto handler = begin(handlers); handler != end(handlers); handler++) {
			(*handler)(args);
		}
	};
};

template <> 
class Event <void> {
	list<function<void()>> handlers;

public:
	void operator+=(function<void()> handler) {
		handlers.push_back( move(handler) );
	};

	void clear() {
		handlers.clear();
	};

	void operator()() {
        for(auto handler = begin(handlers); handler != end(handlers); handler++) {
			(*handler)();
		}
	};
};
