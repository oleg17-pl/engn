#ifndef FOREACHABLE_HPP
#define FOREACHABLE_HPP

#include <functional>

template<typename T>
class IForeachable {
public:
	virtual void forEachMembers(std::function<void(T &)> func) = 0;
};

#endif // !FOREACHABLE_HPP