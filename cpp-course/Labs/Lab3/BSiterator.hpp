#ifndef BSITERATORS_HPP
#define BSITERATORS_HPP

#include <iterator>
#include <utility>

template< typename T >
class BucketStorage;

template< typename T >
class ConstBSIterator
{
  public:
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = T;
	using reference = const T &;
	using pointer = const T *;
	using difference_type = std::ptrdiff_t;
	using size_type = std::size_t;

	using ContainerType = BucketStorage< T >;

  protected:
	const ContainerType *container;
	size_type block_num;
	size_type block_index;
	size_type elem_index;

  public:
	ConstBSIterator() = default;

	ConstBSIterator(const ContainerType *cont, size_type blk_num, size_type blk_index, size_type el_index) :
		container(cont), block_num(blk_num), block_index(blk_index), elem_index(el_index)
	{
	}

	ConstBSIterator(const ConstBSIterator &other) :
		container(other.container), block_num(other.block_num), block_index(other.block_index), elem_index(other.elem_index)
	{
	}

	ConstBSIterator(ConstBSIterator &&other) noexcept :
		container(other.container), block_num(other.block_num), block_index(other.block_index), elem_index(other.elem_index)
	{
	}

	ConstBSIterator &operator=(ConstBSIterator other)
	{
		swap(other);
		return *this;
	}

	ConstBSIterator &operator=(ConstBSIterator &&other) noexcept
	{
		container = other.container;
		block_num = other.block_num;
		block_index = other.block_index;
		elem_index = other.elem_index;
		return *this;
	}

	~ConstBSIterator() = default;

	void swap(ConstBSIterator &other) noexcept
	{
		std::swap(container, other.container);
		std::swap(block_num, other.block_num);
		std::swap(block_index, other.block_index);
		std::swap(elem_index, other.elem_index);
	}

	reference operator*() const { return container->blocks[block_num].data[elem_index]; }

	pointer operator->() const { return &container->blocks[block_num].data[elem_index]; }

	ConstBSIterator &operator++()
	{
		advance();
		return *this;
	}

	ConstBSIterator operator++(int)
	{
		ConstBSIterator tmp = *this;
		++(*this);
		return tmp;
	}

	ConstBSIterator &operator--()
	{
		retreat();
		return *this;
	}

	ConstBSIterator operator--(int)
	{
		ConstBSIterator tmp = *this;
		--(*this);
		return tmp;
	}

	bool operator==(const ConstBSIterator &other) const
	{
		return container == other.container && block_num == other.block_num && elem_index == other.elem_index;
	}

	bool operator!=(const ConstBSIterator &other) const { return !(*this == other); }

	bool operator>(const ConstBSIterator &other) const
	{
		return container == other.container &&
			   ((block_num == other.block_num && elem_index > other.elem_index) || block_num > other.block_num);
	}

	bool operator>=(const ConstBSIterator &other) const { return *this > other || *this == other; }

	bool operator<(const ConstBSIterator &other) const
	{
		return container == other.container &&
			   ((block_num == other.block_num && elem_index < other.elem_index) || block_num < other.block_num);
	}

	bool operator<=(const ConstBSIterator &other) const { return *this < other || *this == other; }

	const ContainerType *getContainer() const { return container; }

	size_type getBlockNum() const { return block_num; }

	size_type getBlockIndex() const { return block_index; }

	size_type getElemIndex() const { return elem_index; }

	void repairBlockIndex()
	{
		if (block_index >= container->block_count || block_num != container->blocks[block_index].block_num)
		{
			size_type index = block_index >= container->block_count ? container->block_count - 1 : block_index - 1;

			while (true)
			{
				if (container->blocks[index].block_num == block_num)
				{
					this->block_index = index;
					break;
				}

				if (index == 0)
				{
					break;
				}

				index--;
			}
		}
	}

  protected:
	void advance()
	{
		repairBlockIndex();

		if ((container->block_capacity - 1 == elem_index) ||
			(container->blocks[block_index].busy_places_right[elem_index + 1] + elem_index + 1 >= container->block_capacity))
		{
			block_index++;
			elem_index = 0;
			if (block_index == container->block_count)
			{
				block_num++;
				return;
			}
			block_num = container->blocks[block_index].block_num;
		}
		else
		{
			elem_index++;
		}

		elem_index += container->blocks[block_index].busy_places_right[elem_index];
	}

	void retreat()
	{
		repairBlockIndex();

		if ((elem_index == 0) || ((elem_index - 1) - container->blocks[block_index].busy_places_left[elem_index - 1] == -1))
		{
			if (block_index == 0)
			{
				return;
			}

			block_index--;
			elem_index = container->block_capacity - 1;
			block_num = container->blocks[block_index].block_num;
		}
		else
		{
			elem_index--;
		}

		elem_index -= container->blocks[block_index].busy_places_left[elem_index];
	}
};

template< typename T >
class BSIterator : public ConstBSIterator< T >
{
  public:
	using Base = ConstBSIterator< T >;
	using iterator_category = typename Base::iterator_category;
	using value_type = typename Base::value_type;
	using reference = T &;
	using pointer = T *;
	using difference_type = typename Base::difference_type;
	using size_type = typename Base::size_type;
	using ContainerType = typename Base::ContainerType;

	BSIterator() = default;

	BSIterator(ContainerType *cont, size_type blk_num, size_type blk_index, size_type el_index) :
		Base(cont, blk_num, blk_index, el_index)
	{
	}

	BSIterator(const BSIterator &other) : Base(other) {}

	BSIterator(BSIterator &&other) noexcept : Base(std::move(other)) {}

	BSIterator &operator=(BSIterator other)
	{
		this->swap(other);
		return *this;
	}

	BSIterator &operator=(BSIterator &&other) noexcept
	{
		Base::operator=(std::move(other));
		return *this;
	}

	~BSIterator() = default;

	BSIterator(const ConstBSIterator< T > &other) : Base(other) {}

	reference operator*() { return const_cast< reference >(Base::operator*()); }

	pointer operator->() { return const_cast< pointer >(Base::operator->()); }

	BSIterator &operator++()
	{
		Base::operator++();
		return *this;
	}

	BSIterator operator++(int)
	{
		BSIterator tmp = *this;
		++(*this);
		return tmp;
	}

	BSIterator &operator--()
	{
		Base::operator--();
		return *this;
	}

	BSIterator operator--(int)
	{
		BSIterator tmp = *this;
		--(*this);
		return tmp;
	}

	const ContainerType *getContainer() const { return Base::getContainer(); }

	size_type getBlockNum() const { return Base::getBlockNum(); }

	size_type getBlockIndex() const { return Base::getBlockIndex(); }

	size_type getElemIndex() const { return Base::getElemIndex(); }

	void repairBlockIndex() { Base::repairBlockIndex(); }
};

#endif
