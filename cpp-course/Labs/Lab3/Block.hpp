#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <utility>

template< typename T >
class Block
{
	template< typename U >
	friend class BucketStorage;

	template< typename U >
	friend class ConstBSIterator;

	template< typename U >
	friend class BSIterator;

  public:
	using size_type = std::size_t;
	using value_type = T;

	size_type block_num;

	Block(size_type blck_nm, size_type cap = 64);

	Block(const Block< T > &other);

	Block &operator=(Block< T > other);

	Block(Block< T > &&other) noexcept;

	Block &operator=(Block< T > &&other) noexcept;

	~Block();

	void clear();

	size_type add(const value_type &value);

	size_type add(value_type &&value);

	void remove(size_type it);

	bool full() const;

	bool empty() const;

	void swap(Block< T > &other) noexcept;

  private:
	value_type *data;
	size_type *busy_places_right;
	size_type *free_places_right;
	size_type *busy_places_left;
	size_type *free_places_left;

  public:
	size_type capacity;
	size_type size;

  private:
	void update_busy_places_left(size_type index, size_type index_for_busy);

	void update_free_places_left(size_type index, size_type index_for_free);

	void update_busy_places_right(size_type index, size_type index_for_busy);

	void update_free_places_right(size_type index, size_type index_for_free);
};

namespace std
{
	template< typename T >
	void swap(Block< T > &lhs, Block< T > &rhs) noexcept
	{
		lhs.swap(rhs);
	}
}	 // namespace std

template< typename T >
Block< T >::Block(size_type blck_nm, size_type cap) : block_num(blck_nm), capacity(cap), size(0)
{
	data = reinterpret_cast< value_type * >(new int8_t[cap * sizeof(value_type)]);
	busy_places_right = new size_type[cap];
	free_places_right = new size_type[cap];
	busy_places_left = new size_type[cap];
	free_places_left = new size_type[cap];

	for (size_type i = 0; i < capacity; ++i)
	{
		busy_places_left[i] = i + 1;
		busy_places_right[i] = capacity - i;
		free_places_left[i] = free_places_right[i] = 1;
	}
}

template< typename T >
Block< T >::Block(const Block< T > &other) : block_num(other.block_num), capacity(other.capacity), size(other.size)
{
	data = reinterpret_cast< value_type * >(new int8_t[capacity * sizeof(value_type)]);

	busy_places_right = new size_type[capacity];
	free_places_right = new size_type[capacity];
	busy_places_left = new size_type[capacity];
	free_places_left = new size_type[capacity];

	try
	{
		for (size_type i = 0; i < capacity; ++i)
		{
			if (other.busy_places_right[i] == 0)
			{
				new (data + i) value_type(other.data[i]);
			}
			busy_places_right[i] = other.busy_places_right[i];
			free_places_right[i] = other.free_places_right[i];
			busy_places_left[i] = other.busy_places_left[i];
			free_places_left[i] = other.free_places_left[i];
		}
	} catch (...)
	{
		delete[] reinterpret_cast< int8_t * >(data);
		delete[] busy_places_right;
		delete[] free_places_right;
		delete[] busy_places_left;
		delete[] free_places_left;
		throw;
	}
}

template< typename T >
Block< T > &Block< T >::operator=(Block< T > other)
{
	if (this != &other && data != other.data)
	{
		std::swap(*this, other);
	}
	return *this;
}

template< typename T >
Block< T >::Block(Block< T > &&other) noexcept :
	block_num(std::move(other.block_num)), capacity(std::move(other.capacity)), size(std::move(other.size)),
	data(std::exchange(other.data, nullptr)), busy_places_right(std::exchange(other.busy_places_right, nullptr)),
	free_places_right(std::exchange(other.free_places_right, nullptr)),
	busy_places_left(std::exchange(other.busy_places_left, nullptr)),
	free_places_left(std::exchange(other.free_places_left, nullptr))
{
}

template< typename T >
Block< T > &Block< T >::operator=(Block< T > &&other) noexcept
{
	if (this != &other)
	{
		block_num = std::move(other.block_num);
		capacity = std::move(other.capacity);
		size = std::move(other.size);

		data = std::move(other.data);
		busy_places_right = std::move(other.busy_places_right);
		free_places_right = std::move(other.free_places_right);
		busy_places_left = std::move(other.busy_places_left);
		free_places_left = std::move(other.free_places_left);
	}
	return *this;
}

template< typename T >
Block< T >::~Block()
{
	clear();
	delete[] reinterpret_cast< int8_t * >(data);
	delete[] busy_places_right;
	delete[] free_places_right;
	delete[] busy_places_left;
	delete[] free_places_left;
}

template< typename T >
void Block< T >::clear()
{
	size_type index = 0;
	while (index != capacity)
	{
		if (busy_places_right[index] == 0)
		{
			data[index].~value_type();
			++index;
		}
		else
		{
			index += busy_places_right[index];
		}
	}
}

template< typename T >
typename Block< T >::size_type Block< T >::add(const value_type &value)
{
	size_type index = busy_places_right[0] == 0 ? free_places_right[0] : 0;
	if (index == capacity)
	{
		throw std::overflow_error("Block is overflow.");
	}

	size++;
	new (data + index) value_type(value);
	busy_places_right[index] = busy_places_left[index] = 0;

	update_busy_places_left(index, index);
	update_free_places_left(index, index - free_places_left[index]);
	update_busy_places_right(index, index);
	update_free_places_right(index, index + free_places_right[index]);

	return index;
}

template< typename T >
typename Block< T >::size_type Block< T >::add(value_type &&value)
{
	size_type index = busy_places_right[0] == 0 ? free_places_right[0] : 0;
	if (index == capacity)
	{
		throw std::overflow_error("Block is overflow.");
	}

	size++;
	new (data + index) value_type(std::move(value));
	busy_places_right[index] = busy_places_left[index] = 0;

	update_busy_places_left(index, index);
	update_free_places_left(index, index - free_places_left[index]);
	update_busy_places_right(index, index);
	update_free_places_right(index, index + free_places_right[index]);

	return index;
}

template< typename T >
void Block< T >::remove(size_type it)
{
	if (it >= capacity || busy_places_right[it] != 0)
	{
		throw std::invalid_argument("Attempt to erase a non-exist element or out of range.");
	}

	size--;
	data[it].~value_type();

	size_type index_for_busy_right = it < (capacity - 1) ? ((it + 1) + busy_places_right[it + 1]) : capacity;
	size_type index_for_busy_left = it > 0 ? ((it - 1) - busy_places_left[it - 1]) : -1;

	busy_places_right[it] = index_for_busy_right - it;
	busy_places_left[it] = it - index_for_busy_left;

	update_busy_places_left(it, index_for_busy_left);
	update_free_places_left(it, it);
	update_busy_places_right(it, index_for_busy_right);
	update_free_places_right(it, it);
}

template< typename T >
bool Block< T >::full() const
{
	return capacity == size;
}

template< typename T >
bool Block< T >::empty() const
{
	return size == 0;
}

template< typename T >
void Block< T >::swap(Block< T > &other) noexcept
{
	std::swap(capacity, other.capacity);
	std::swap(size, other.size);
	std::swap(block_num, other.block_num);

	for (size_type i = 0; i < capacity; ++i)
	{
		std::swap(busy_places_right[i], other.busy_places_right[i]);
		std::swap(free_places_right[i], other.free_places_right[i]);
		std::swap(busy_places_left[i], other.busy_places_left[i]);
		std::swap(free_places_left[i], other.free_places_left[i]);
	}
}

template< typename T >
void Block< T >::update_busy_places_left(size_type index, size_type index_for_busy)
{
	index++;
	for (; index < capacity; ++index)
	{
		if (busy_places_left[index] == 0)
		{
			break;
		}
		busy_places_left[index] = index - index_for_busy;
	}
}

template< typename T >
void Block< T >::update_free_places_left(size_type index, size_type index_for_free)
{
	index++;
	for (; index < capacity; ++index)
	{
		free_places_left[index] = index - index_for_free;
		if (busy_places_left[index] != 0)
		{
			break;
		}
	}

	if (index < capacity && index_for_free < free_places_left[index])
	{
		free_places_left[index] = index_for_free;
	}
}

template< typename T >
void Block< T >::update_busy_places_right(size_type index, size_type index_for_busy)
{
	if (index == 0)
		return;
	index--;
	for (; index < capacity; --index)
	{
		if (busy_places_right[index] == 0)
		{
			break;
		}
		busy_places_right[index] = index_for_busy - index;
	}
}

template< typename T >
void Block< T >::update_free_places_right(size_type index, size_type index_for_free)
{
	if (index == 0)
		return;
	index--;
	for (; index < capacity; --index)
	{
		free_places_right[index] = index_for_free - index;
		if (busy_places_right[index] != 0)
		{
			break;
		}
	}

	if (index < capacity && index_for_free < free_places_right[index])
	{
		free_places_right[index] = index_for_free;
	}
}

#endif
