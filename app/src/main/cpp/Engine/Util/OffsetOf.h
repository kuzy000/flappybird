#pragma once

#include <cstddef>
#include <type_traits>

namespace Flappy {
namespace Util {

template <typename T, typename Class>
constexpr std::size_t offset_of(T Class::* member) {
	static_assert(std::is_standard_layout<Class>::value, "is only available for standart-layout classes");
	
	return reinterpret_cast<std::size_t>(&(static_cast<Class*>(nullptr)->*member)) - reinterpret_cast<std::size_t>(nullptr);
}

template <typename T, typename Class>
constexpr std::size_t offset_of_unsafe(T Class::* member) {
	return reinterpret_cast<std::size_t>(&(static_cast<Class*>(nullptr)->*member)) - reinterpret_cast<std::size_t>(nullptr);
}

}
}

