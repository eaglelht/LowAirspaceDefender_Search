#ifndef IO_BUFFER_H_
#define IO_BUFFER_H_


template <class T>
struct io_buffer 
{
	T* buffer_;
	int size_;
	int max_size_;

	inline T* get_write_pos()
	{
		return (buffer_);
	}

	inline int get_write_size()
	{
		return size_;
	}

	inline T* get_read_pos()
	{
		return (buffer_ + size_);
	}
	inline int get_read_size()
	{
		return (max_size_ - size_);
	}
	inline void add_size(int size)
	{
		size_ += size;
	}

	io_buffer(int size)
		: max_size_(size)
	{
		buffer_ = new T[max_size_];
		size_ = 0;
	}

	~io_buffer()
	{
		if (buffer_)
		{
			delete[] buffer_;
			buffer_ = 0;
		}
	}
};

#endif // IO_BUFFER_H_
