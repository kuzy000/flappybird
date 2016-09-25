#pragma once

#include "Engine/Util/MoveOnly.h"
#include "Engine/Util/Assert.h"

#include <type_traits>
#include <set>
#include <functional>

namespace Flappy {

template <typename F, typename Friend = void, typename Tag = void, bool CopyCtr = false>
class Event {
	static_assert(!std::is_same<F, F>::value, "Event<...> - incorrect template arguments");
};

template <typename Ret, typename... Args, typename Tag, typename Friend, bool CopyCtr>
class Event<Ret(Args...), Friend, Tag, CopyCtr> : public std::enable_if<!CopyCtr, Util::MoveOnly>::type {
	friend Friend;
public:
	class Id;
private:
	class Handler {
	public:
		Handler(std::function<Ret(Args...)> fun, bool disposable)
			: fun {std::move(fun)}, disposable {disposable}, index {next_index++} {}
		
		bool operator <(const Handler& rhs) const {
			return index < rhs.index;
		}
		
		const std::function<Ret(Args...)> fun;
		const bool disposable;
	private:
		const std::size_t index;
		static std::size_t next_index;
	};
public:
	class Id : Util::MoveOnly {
		friend class Event<Ret(Args...), Friend, Tag, CopyCtr>;
	public:
		using EventType = Event<Ret(Args...), Friend, Tag, CopyCtr>;
	private:
		Id() = default;
		typename std::set<Handler>::iterator iterator;
	};
	
	void remove(Id&& id) {
		FLAPPY_ASSERT(id.iterator != handlers.end(), "Event<...>::remove - invalid Event<...>::Id (already moved?)");
		handlers.erase(id.iterator);
		id.iterator = handlers.end();
	}
	
	void clear() {
		handlers.clear();
	}
	
	template <typename F>
	Id watch(F&& fun) {
		return add(std::forward<F>(fun), false);
	}
	
	template <typename F>
	Id watch_once(F&& fun) {
		return add(std::forward<F>(fun), true);
	}
	
protected:
	void call(const Args&... args) {
		for(auto it = handlers.begin(); it != handlers.end();) {
			it->fun(args...);
			if(it->disposable) {
				it = handlers.erase(it);
			}
			else {
				++it;
			}
		}
	}

	template <typename F>
	Id add(F&& fun, bool disposable) {
		Handler h {std::forward<F>(fun), disposable};
		
		auto p = handlers.insert(h);
		FLAPPY_ASSERT(p.second, "Event<...>::add - already inserted");
		Id id;
		id.iterator = p.first;
		
		return id;
	}
	
	std::set<Handler> handlers;
};

template <typename Ret, typename... Args, typename Tag, typename Friend, bool CopyCtr>
std::size_t Event<Ret(Args...), Friend, Tag, CopyCtr>::Handler::next_index = 0;

}
