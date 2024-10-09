#pragma once
#include <iostream>


template<typename T, typename Alloc = std::allocator<T>>
class Vector
{
public:
	Vector();
	Vector(const size_t size, const T& value = T(), const Alloc& alloc = Alloc());
	Vector(const Vector<T, Alloc>& copy_v);
	~Vector();



	void push_back(const T& value);
	void pop_back();

	void reserve(const size_t n);
	void resize(const size_t size, const T& value = T());

	size_t capacity() const;
	size_t size() const;
	bool empty() const;

	T& operator[](const size_t index);
	Vector<T, Alloc> operator +(const Vector<T, Alloc>& other);
	Vector<T, Alloc>& operator =(const Vector<T, Alloc>& other);
	T& at(const size_t index);

	

private:
	T* arr;
	size_t m_size;
	size_t m_cap;
	Alloc m_allocator;

	using alloc_traits = std::allocator_traits < Alloc >;
};

template<typename T, typename Alloc>
Vector<T, Alloc>::Vector() : arr{ nullptr }, m_size{ 0 }, m_cap{ 0 }, m_allocator{ Alloc() } {}

template<typename T, typename Alloc>
Vector<T, Alloc>::Vector(const size_t size, const T& value, const Alloc& alloc) : m_size{ 0 }, m_cap{ 0 }, m_allocator{ alloc }
{
	reserve(size);
	for (size_t i = 0; i < size; ++i)
	{
		alloc_traits::construct(m_allocator, arr + i, value);
		//m_allocator.construct(arr + i, value);
	}
	m_size = size;
}

template<typename T, typename Alloc>
Vector<T, Alloc>::Vector(const Vector<T, Alloc>& copy_v): m_allocator{alloc_traits::select_on_container_copy_construction(copy_v.m_allocator)}
{
	reserve(copy_v.m_size);
	for (size_t i = 0; i < copy_v.m_size; ++i)
	{
		alloc_traits::construct(m_allocator, arr + i, *(copy_v.arr+i));
		//m_allocator.construct(arr + i, value);
	}
	this->m_size = copy_v.m_size;
}

template<typename T, typename Alloc>
Vector<T, Alloc>::~Vector()
{
	for (size_t i{ 0 }; i < m_size; ++i)
	{
		alloc_traits::destroy(m_allocator, arr + i);
		//m_allocator.destroy(arr + i);
		//(arr + i)->~T();
	}

	m_allocator.deallocate(arr, m_cap);
	//delete[] reinterpret_cast<char*>(arr);
}


//Универсальная ссылка todo
template<typename T, typename Alloc>
void Vector<T, Alloc>::push_back(const T& value)
{
	if (m_size == 0 && m_cap == 0) reserve(1);
	if (m_size == m_cap) reserve(m_cap * 2);
	alloc_traits::construct(m_allocator, arr + m_size, value);
	//m_allocator.construct(arr + m_size, value);
	//new (arr + m_size) T(value);
	++m_size;
}

template<typename T, typename Alloc>
void Vector<T, Alloc>::pop_back()
{
	--m_size;
	alloc_traits::destroy(m_allocator, arr + m_size);
	//m_allocator.destroy(arr + m_size);
	//(arr + m_size)->~T();
}

template<typename T, typename Alloc>
void Vector<T, Alloc>::reserve(const size_t n)
{
	if (n <= m_cap) return;

	T* temp_arr = m_allocator.allocate(n);
	//T* temp_arr = reinterpret_cast<T*>(new char[n * sizeof(T)]);
	size_t pos{ 0 };

	try
	{
		for (; pos < m_size; ++pos)
		{
			alloc_traits::construct(m_allocator, temp_arr + pos, arr[pos]);
			//std::uninitialized_copy(arr, arr + m_size, temp_arr);
			//m_allocator.construct(temp_arr + pos, arr[pos]);
			//new (temp_arr + pos) T(arr[pos]);
		}
	}
	catch (const std::exception&)
	{
		//Очистка выделенной памяти.
		for (size_t i{ 0 }; i < pos; ++i)
		{
			alloc_traits::destroy(m_allocator, temp_arr + i);
			//m_allocator.destroy(temp_arr + i);
			//(temp_arr + pos)->~T();
		}

		m_allocator.deallocate(temp_arr, n);
		//delete[] reinterpret_cast<char*>(temp_arr);
		throw;
	}
	for (size_t i = 0; i < m_size; ++i)
	{
		alloc_traits::destroy(m_allocator, arr + i);
		//m_allocator.destroy(arr + i);
	}

	m_allocator.deallocate(arr, m_cap);

	arr = temp_arr;
	m_cap = n;
}

template<typename T, typename Alloc>
void Vector<T, Alloc>::resize(const size_t size, const T& value)
{
	if (size > m_cap) reserve(size);
	for (size_t i = m_size; i < size; ++i)
	{
		alloc_traits::construct(m_allocator, arr + i, value);
		//m_allocator.construct(arr + i, value);
		//new (arr + i) T(value);
	}
	m_size = size;

}

template<typename T, typename Alloc>
size_t Vector<T, Alloc>::capacity() const
{
	return m_cap;
}

template<typename T, typename Alloc>
size_t Vector<T, Alloc>::size() const
{
	return m_size;
}

template<typename T, typename Alloc>
bool Vector<T, Alloc>::empty() const
{
	return m_size == 0;
}

template<typename T, typename Alloc>
T& Vector<T, Alloc>::operator[](const size_t index)
{
	return *(arr + index);
}

template<typename T, typename Alloc>
Vector<T, Alloc> Vector<T, Alloc>::operator+(const Vector<T, Alloc>& other)
{
	Vector<T,Alloc> newVector = Vector(this->m_size > other.m_size ? this : other);
	Vector<T, Alloc>& ptrMinVector = this->m_size < other.m_size ? this : other;
	

	for (size_t i = 0; i < ptrMinVector.m_size; i++)
	{
		*(newVector.arr + i) = *(ptrMinVector.arr + i);
	}

	return Vector<T, Alloc>();
}


template<typename T, typename Alloc>
Vector<T, Alloc>& Vector<T, Alloc>::operator=(const Vector<T, Alloc>& other)
{
	for (size_t i = 0; i < m_size; ++i)
	{
		alloc_traits::destroy(m_allocator, arr + i);
	}
	alloc_traits::deallocate(m_allocator, arr, m_cap);

	if (alloc_traits::propagate_on_container_copy_assignment::value && m_allocator != other.m_allocator)
	{
		m_allocator = other.m_allocator;
	}
	size_t pos = 0;
	try
	{
		reserve(other.m_size);
		for (; pos < other.m_size; ++pos)
		{
			
			alloc_traits::construct(m_allocator, arr + pos , other.arr[pos]);
		}
	}
	catch (const std::exception&)
	{
		//Очистка выделенной памяти.
		for (size_t i{ 0 }; i < pos; ++i)
		{
			alloc_traits::destroy(m_allocator, arr+i);
			//m_allocator.destroy(temp_arr + i);
			//(temp_arr + pos)->~T();
		}

		alloc_traits::deallocate(m_allocator, arr ,m_cap);
		
		throw;
	}

}

template<typename T, typename Alloc>
T& Vector<T, Alloc>::at(const size_t index)
{
	if (index >= m_size) throw std::out_of_range("out of range");
	return *(arr + index);
}
