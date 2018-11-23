#ifndef MUTEX_LIST_H_
#define MUTEX_LIST_H_
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/lock_types.hpp>

template<class T>
class mutex_list
{
	std::list<T> list_t_;
	boost::shared_mutex mutex_t_;
public:
	inline void push(T t)
	{
		boost::unique_lock<boost::shared_mutex> lock(mutex_t_);
		list_t_.push_back(t);
	}

	inline T pop()
	{
		boost::unique_lock<boost::shared_mutex> lock(mutex_t_);
		T t;
		if (list_t_.size() > 0)
		{
			t = list_t_.front();
			list_t_.pop_front();
		}
		return t;
	}

	inline void clear()
	{
		boost::unique_lock<boost::shared_mutex> lock(mutex_t_);
		list_t_.clear();
	}

	inline bool is_empty()
	{
		boost::unique_lock<boost::shared_mutex> lock(mutex_t_);
		return list_t_.empty();
	}
};

#endif // MUTEX_LIST_H_