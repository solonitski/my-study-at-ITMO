// ---------------------------------------------------------------------------------------------------------------------
// START OF INTERFACE
// ---------------------------------------------------------------------------------------------------------------------
#ifndef BUCKET_STORAGE_HPP
#define BUCKET_STORAGE_HPP

#include "BSiterator.cpp"
#include "Block.cpp"
#include "SVector.cpp"

#include <iostream>

template< typename T >
class BucketStorage
{
  public:
	using value_type = T;
	using reference = T &;
	using const_reference = const T &;
	using iterator = BSIterator< value_type >;
	using const_iterator = ConstBSIterator< T >;
	using difference_type = std::ptrdiff_t;
	using size_type = std::size_t;
	using pointer = T *;

  public:
	BucketStorage(size_type blck_cap = 64);
	BucketStorage(const BucketStorage< T > &other);
	BucketStorage(BucketStorage< T > &&other) noexcept;
	BucketStorage &operator=(const BucketStorage< T > &other);
	BucketStorage &operator=(BucketStorage< T > &&other) noexcept;
	~BucketStorage();

	iterator insert(const value_type &value);
	iterator insert(value_type &&value);
	iterator erase(const_iterator it);

	bool empty() const noexcept;
	size_type size() const noexcept;
	size_type capacity() const noexcept;
	void shrink_to_fit();
	void clear();
	void swap(BucketStorage< T > &other) noexcept;

	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	const_iterator cbegin() const noexcept;
	iterator end() noexcept;
	const_iterator end() const noexcept;
	const_iterator cend() const noexcept;
	iterator get_to_distance(iterator it, const difference_type distance);

  private:
	size_type block_capacity;
	SVector< Block< value_type > > blocks;
	size_type block_count;
	size_type sz;

	template< typename U >
	friend class ConstBSIterator;

	template< typename U >
	friend class BSIterator;
};

// ---------------------------------------------------------------------------------------------------------------------
// START OF IMPLEMENTATION
// ---------------------------------------------------------------------------------------------------------------------

template< typename T >
BucketStorage< T >::BucketStorage(size_type blck_cap) : block_capacity(blck_cap), block_count(0), sz(0)
{
}

template< typename T >
BucketStorage< T >::BucketStorage(const BucketStorage< T > &other) :
	block_capacity(other.block_capacity), blocks(std::move(other.blocks)), block_count(other.block_count), sz(other.sz)
{
}

template< typename T >
BucketStorage< T >::BucketStorage(BucketStorage< T > &&other) noexcept :
	block_capacity(other.block_capacity), blocks(std::move(other.blocks)), block_count(other.block_count), sz(other.sz)
{
	other.block_capacity = 0;
	other.block_count = 0;
	other.sz = 0;
}

template< typename T >
BucketStorage< T > &BucketStorage< T >::operator=(const BucketStorage< T > &other)
{
	if (this != &other)
	{
		BucketStorage< value_type > copy(other);
		swap(copy);
	}
	return *this;
}

template< typename T >
BucketStorage< T > &BucketStorage< T >::operator=(BucketStorage< T > &&other) noexcept
{
	if (this != &other)
	{
		swap(other);
		other.clear();
	}
	return *this;
}

template< typename T >
BucketStorage< T >::~BucketStorage()
{
	blocks.clear();
}

template< typename T >
typename BucketStorage< T >::iterator BucketStorage< T >::insert(const value_type &value)
{
	for (int i = 0; i < block_count; ++i)
	{
		if (!blocks[i].full())
		{
			sz++;
			size_type element_index = blocks[i].add(value);
			return iterator(this, blocks[i].block_num, i, element_index);
		}
	}

	size_t blk_nm = sz == 0 ? 0 : blocks[block_count - 1].block_num + 1;
	blocks.emplace_back(blk_nm, block_capacity);
	block_count++;

	size_type element_index = blocks[block_count - 1].add(value);
	sz++;

	return iterator(this, blk_nm, block_count - 1, element_index);
}

template< typename T >
typename BucketStorage< T >::iterator BucketStorage< T >::insert(value_type &&value)
{
	for (int i = 0; i < block_count; ++i)
	{
		if (!blocks[i].full())
		{
			sz++;
			size_type element_index = blocks[i].add(std::move(value));
			return iterator(this, blocks[i].block_num, i, element_index);
		}
	}

	size_t blk_nm = sz == 0 ? 0 : blocks[block_count - 1].block_num + 1;
	blocks.emplace_back(blk_nm, block_capacity);

	block_count++;

	size_type element_index = blocks[block_count - 1].add(std::move(value));
	sz++;

	return iterator(this, blk_nm, block_count - 1, element_index);
}

template< typename T >
typename BucketStorage< T >::iterator BucketStorage< T >::erase(const_iterator it)
{
	if (it == this->cend())
	{
		return this->cend();
	}

	iterator copy_it = it;
	copy_it.repairBlockIndex();

	size_type blk_index = copy_it.getBlockIndex();
	size_type elm_index = copy_it.getElemIndex();

	++copy_it;

	blocks[blk_index].remove(elm_index);
	sz--;

	if (blocks[blk_index].empty())
	{
		blocks.erase(0);
		block_count--;
	}

	return copy_it;
}

template< typename T >
bool BucketStorage< T >::empty() const noexcept
{
	return begin() == end();
}

template< typename T >
typename BucketStorage< T >::size_type BucketStorage< T >::size() const noexcept
{
	return sz;
}

template< typename T >
typename BucketStorage< T >::size_type BucketStorage< T >::capacity() const noexcept
{
	return block_count * block_capacity;
}

template< typename T >
void BucketStorage< T >::shrink_to_fit()
{
	BucketStorage< value_type > new_storage(block_capacity);

	for (auto it = begin(); it != end(); ++it)
	{
		new_storage.insert(*it);
	}

	this->swap(new_storage);
}

template< typename T >
void BucketStorage< T >::clear()
{
	blocks.clear();
	block_count = 0;
	sz = 0;
}

template< typename T >
void BucketStorage< T >::swap(BucketStorage< T > &other) noexcept
{
	std::swap(block_capacity, other.block_capacity);
	std::swap(blocks, other.blocks);
	std::swap(block_count, other.block_count);
	std::swap(sz, other.sz);
}

template< typename T >
typename BucketStorage< T >::iterator BucketStorage< T >::begin() noexcept
{
	if (block_count == 0)
	{
		return iterator(this, 0, 0, 0);
	}

	size_type it_block_num = blocks[0].block_num;
	size_type it_el_index = blocks[0].busy_places_right[0];

	return iterator(this, it_block_num, 0, it_el_index);
}

template< typename T >
typename BucketStorage< T >::const_iterator BucketStorage< T >::begin() const noexcept
{
	return cbegin();
}

template< typename T >
typename BucketStorage< T >::const_iterator BucketStorage< T >::cbegin() const noexcept
{
	if (block_count == 0)
	{
		return const_iterator(this, 0, 0, 0);
	}

	size_type it_block_num = blocks[0].block_num;
	size_type it_el_index = blocks[0].busy_places_right[0];

	return const_iterator(this, it_block_num, 0, it_el_index);
}

template< typename T >
typename BucketStorage< T >::iterator BucketStorage< T >::end() noexcept
{
	if (block_count == 0)
	{
		return iterator(this, 0, 0, 0);
	}

	size_type it_block_num = blocks[block_count - 1].block_num;

	return iterator(this, it_block_num + 1, block_count, 0);
}

template< typename T >
typename BucketStorage< T >::const_iterator BucketStorage< T >::end() const noexcept
{
	return cend();
}

template< typename T >
typename BucketStorage< T >::const_iterator BucketStorage< T >::cend() const noexcept
{
	if (block_count == 0)
	{
		return const_iterator(this, 0, 0, 0);
	}

	size_type it_block_num = blocks[block_count - 1].block_num;

	return const_iterator(this, it_block_num + 1, block_count, 0);
}

template< typename T >
typename BucketStorage< T >::iterator BucketStorage< T >::get_to_distance(iterator it, const difference_type distance)
{
	std::advance(it, distance);
	return it;
}

#endif
