#pragma once
#include <list>
#include <functional>
using namespace std;

template<typename ...Args>
class Event {
	list<function<void(Args...)> handlers;
public:
	operator+=(function<void(Args...)> handler) {		
        if ( end( handlers ) == find(begin( handlers ), end( handlers ), handler ) )
        {
			handlers.push_back( move(handler) );
        }
	};

	operator-=(function<void(Args...)> handler) {
		auto h = find(begin( handlers ), end( handlers ), handler);
		if ( end( handlers ) !=  h)
        {
			handlers.remove( h );
        }
	};

	void operator()(Args &&	... args) {
        for(auto& handler : handlers)
			handler(forward<Args>(args)...);
	};
};