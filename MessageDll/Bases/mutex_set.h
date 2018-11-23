#ifndef MUTEX_SET_H_
#define MUTEX_SET_H_
#include <set>
#include <boost/thread/shared_mutex.hpp>

template<class T>
class mutex_set
{
protected:
	std::set<T> set_t_;
	boost::shared_mutex mutex_t_;
public:
	inline void insert(T t)
	{
		boost::unique_lock<boost::shared_mutex> lock(mutex_t_);
		set_t_.insert(t);
	}

	inline void erase(T t)
	{
		boost::unique_lock<boost::shared_mutex> lock(mutex_t_);
		set_t_.erase(t);
	}

	inline void clear()
	{
		boost::unique_lock<boost::shared_mutex> lock(mutex_t_);
		set_t_.clear();
	}

	inline bool is_empty()
	{
		boost::shared_lock<boost::shared_mutex> lock(mutex_t_);
		return set_t_.empty();
	}
};

#endif // MUTEX_SET_H_
