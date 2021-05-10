#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <mutex>
#include <condition_variable>
#include <iostream>

class sem 
{
private:
	int count;
	int wakeups;
	std::mutex mutex;
	std::condition_variable condition;
public:
	sem(int value = 0) : count{ value }, wakeups{ 0 } {}
	void wait() {
		std::unique_lock<std::mutex> lock{ mutex };
		if (--count < 0) { // count is not enough ?
			condition.wait(lock, [&]()->bool { return wakeups > 0; }); // suspend and wait ...
			--wakeups; // ok, me wakeup !
		}
	}
	void post() {
		std::lock_guard<std::mutex> lock{ mutex };
		if (++count <= 0) { // have some thread suspended ?
			++wakeups;
			condition.notify_one(); // notify one !
		}
	}
};
#endif
