#ifndef SVECTOR_HPP
#define SVECTOR_HPP

#include <type_traits>

#include <cstddef>
#include <cstdint>
#include <new>
#include <stdexcept>
#include <utility>

template< typename T >
class SVector
{
  public:
	SVector();

	~SVector();

	SVector(const SVector< T > &other);

	SVector(SVector< T > &&other) noexcept;

	SVector &operator=(SVector< T > other);

	SVector< T > &operator=(SVector< T > &&other);

	void swap(SVector< T > &other) noexcept;

	friend void swap(SVector< T > &lhs, SVector< T > &rhs) noexcept { lhs.swap(rhs); }

	size_t capacity() const;

	size_t size() const;

	void clear();

	void reserve(size_t newcap);

	void push_back(const T &value);

	template< typename... Args >
	void emplace_back(Args &&...args);

	void erase(size_t index);

	T &operator[](size_t index) const;

  private:
	size_t cap_;
	size_t sz_;
	T *arr_;
};

template< typename T >
SVector< T >::SVector() : cap_(0), sz_(0), arr_(reinterpret_cast< T * >(new int8_t[cap_ * sizeof(T)]))
{
}

template< typename T >
SVector< T >::SVector(SVector< T > &&other) noexcept : cap_(0), sz_(0), arr_(nullptr)
{
	swap(other);
}

template< typename T >
SVector< T > &SVector< T >::operator=(SVector< T > &&other)
{
	swap(other);
	return *this;
}

template< typename T >
SVector< T >::~SVector()
{
	for (size_t i = 0; i < sz_; ++i)
	{
		(arr_ + i)->~T();
	}
	delete[] reinterpret_cast< int8_t * >(arr_);
}

template< typename T >
SVector< T >::SVector(const SVector< T > &other) : arr_(nullptr), cap_(0), sz_(0)
{
	if (other.sz_ > 0)
	{
		reserve(other.sz_);
		for (size_t i = 0; i < other.sz_; ++i)
		{
			new (arr_ + i) T(other.arr_[i]);
		}
		sz_ = other.sz_;
	}
}

template< typename T >
SVector< T > &SVector< T >::operator=(SVector< T > other)
{
	swap(other);
	return *this;
}

template< typename T >
void SVector< T >::swap(SVector< T > &other) noexcept
{
	std::swap(arr_, other.arr_);
	std::swap(cap_, other.cap_);
	std::swap(sz_, other.sz_);
}

template< typename T >
size_t SVector< T >::capacity() const
{
	return cap_;
}

template< typename T >
size_t SVector< T >::size() const
{
	return sz_;
}

template< typename T >
void SVector< T >::clear()
{
	for (size_t i = 0; i < sz_; ++i)
	{
		(arr_ + i)->~T();
	}
	sz_ = 0;
}

template< typename T >
void SVector< T >::reserve(size_t newcap)
{
	if (newcap <= cap_)
		return;

	T *newarr = reinterpret_cast< T * >(new int8_t[newcap * sizeof(T)]);

	size_t index = 0;
	try
	{
		if (std::is_move_constructible< T >::value)
		{
			for (; index < sz_; ++index)
			{
				new (newarr + index) T(std::move(arr_[index]));
			}
		}
		else if (std::is_copy_constructible< T >::value)
		{
			for (; index < sz_; ++index)
			{
				new (newarr + index) T(arr_[index]);
			}
		}
		else
		{
			throw std::runtime_error(
				"Type T does not have a default constructor, move constructor, or copy "
				"constructor");
		}
	} catch (...)
	{
		for (size_t i = 0; i < index; ++i)
		{
			(newarr + i)->~T();
		}
		delete[] reinterpret_cast< int8_t * >(newarr);
		throw;
	}

	delete[] reinterpret_cast< int8_t * >(arr_);

	arr_ = newarr;
	cap_ = newcap;
}

template< typename T >
void SVector< T >::push_back(const T &value)
{
	if (sz_ == cap_)
	{
		reserve(cap_ > 0 ? cap_ * 2 : 1);
	}
	new (arr_ + sz_) T(value);
	++sz_;
}

template< typename T >
template< typename... Args >
void SVector< T >::emplace_back(Args &&...args)
{
	if (sz_ == cap_)
	{
		reserve(cap_ > 0 ? cap_ * 2 : 1);
	}
	new (arr_ + sz_) T(std::forward< Args >(args)...);
	++sz_;
}

template< typename T >
void SVector< T >::erase(size_t index)
{
	if (index >= sz_)
	{
		throw std::out_of_range("Error. Out of range.");
	}

	for (size_t i = index; i < sz_ - 1; ++i)
	{
		std::swap(arr_[i], arr_[i + 1]);
	}

	--sz_;
}

template< typename T >
T &SVector< T >::operator[](size_t index) const
{
	return arr_[index];
}

namespace std
{
	template< typename T >
	void swap(SVector< T > &lhs, SVector< T > &rhs)
	{
		lhs.swap(rhs);
	}
}	 // namespace std

#endif
