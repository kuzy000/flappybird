#pragma once

#include <iostream>
#include <sstream>
#include <functional>
#include <cstddef>
#include <cassert>

#include "Util/MoveOnly.h"

namespace Flappy {

class IndentionStreamBuf : public std::streambuf {
public:
	void inc() {
		indention_level++;

		current += indention_string;
	}

	void dec() {
		assert(indention_level > 0);
		indention_level--;

		current.erase(current.size() - indention_string.size());
	}

	void setProxy(std::streambuf* value) {
		buf = value;
	}

	std::streambuf* getProxy() const {
		return buf;
	}

protected:
	virtual int overflow(int c) override final {
		if(start_of_line && c != '\n') {
			buf->sputn(current.data(), static_cast<std::streamsize>(current.size()));
		}
		start_of_line = c == '\n';

		return buf->sputc(static_cast<char>(c));
	}

private:
	const std::string indention_string {"    "};
	std::uint_fast8_t indention_level = 0;

	bool start_of_line = true;
	std::string current;

	std::streambuf* buf;
};

class CustomLog : public Util::MoveOnly {
public:
	CustomLog(std::ostream& os)
		: os {os} {}

	template <typename... Args>
	void operator ()(Args&&... args) {
		impl(std::forward<Args>(args)...);
	}

	template <typename... Args>
	void indent(Args&&... args) {
		indent_buf.inc();
		operator ()(std::forward<Args>(args)...);
		indent_buf.dec();
	}

	template <typename F>
	void indent_in(F f) {
		indent_buf.inc();
		f();
		indent_buf.dec();
	}

private:
	std::ostream& os;
	IndentionStreamBuf indent_buf;

	void impl() {}

	template <typename Head, typename... Tail>
	void impl(Head&& head, Tail&&... tail) {
		indent_buf.setProxy(os.rdbuf());

		os.rdbuf(&indent_buf);
		os << std::forward<Head>(head);
		os.rdbuf(indent_buf.getProxy());

		impl(std::forward<Tail>(tail)...);
	}
};


namespace Log {

static CustomLog error {std::cerr};

static CustomLog info {std::cout};

}

}
