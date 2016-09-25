#pragma once

namespace Flappy {
namespace Util {

class MoveOnly {
public:
	MoveOnly() = default;
	
	MoveOnly(const MoveOnly&) = delete;
	MoveOnly& operator =(const MoveOnly&) = delete;

	MoveOnly(MoveOnly&&) = default;
	MoveOnly& operator =(MoveOnly&&) = default;
};

}
}
