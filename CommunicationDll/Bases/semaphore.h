#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>    
#include <boost/thread/lock_types.hpp>
#include <boost/thread.hpp> 

class semaphore
{
	unsigned int count_;
	boost::mutex mutex_;
	boost::condition_variable condition_;

public:
	explicit semaphore(unsigned int initial) : count_(initial) {}

	void signal()
	{
		{
			boost::lock_guard<boost::mutex> lock(mutex_);
			++count_;
		}
		condition_.notify_one();
	}

	void wait()
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		while (count_ == 0)
		{
			condition_.wait(lock);
		}
		--count_;
	}
};

#endif // SEMAPHORE_H_
