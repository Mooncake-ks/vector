#include <iostream>
//#include <vector>



template<typename T, typename Alloc = std::allocator<T>>
class Vector
{
public:
	Vector();
	Vector(const size_t size, const T& value=T(),const Alloc & alloc = Alloc());
	~Vector();

	

	void push_back(const T& value);
	void pop_back();

	void reserve(const size_t n);
	void resize(const size_t size, const T& value = T());

	size_t capacity() const;
	size_t size() const;
	bool empty() const;

	T& operator[](const size_t index);
	T& at(const size_t index);

private:
	T* arr;
	size_t m_size;
	size_t m_cap;
	Alloc m_allocator;
};

template<typename T>
void print(Vector<T>& vc)
{
	for (size_t i = 0; i < vc.size(); ++i)
	{
		std::cout << vc[i] << std::endl;
	}
}

int main()
{
	Vector<int> a(5);
	a.push_back(1);
	a.push_back(2);
	a.push_back(3);
	a.push_back(4);

	std::cout << a.size() << "\t " << a.capacity() << std::endl;

	a.resize(10, 4);
	a.push_back(77777);
	std::cout << "Resize " << a.size() << "\t " << a.capacity() << "\n" << std::endl;
	
	print(a);


    return 0;
}


template<typename T, typename Alloc>
Vector<T, Alloc>::Vector() : arr{ nullptr }, m_size{ 0 }, m_cap{ 0 }, m_allocator{Alloc()} {}

template<typename T, typename Alloc>
Vector<T,Alloc>::Vector(const size_t size, const T& value, const Alloc& alloc) : m_size{ 0 }, m_cap{ 0 }, m_allocator{alloc}
{
	reserve(size);
	for (size_t i = 0; i < size; ++i)
	{
		m_allocator.construct(arr + i, value);
	}
}

template<typename T, typename Alloc>
Vector<T, Alloc>::~Vector()
{
	for (size_t i{ 0 }; i < m_size; ++i)
	{
		m_allocator.destroy(arr + i);
		//(arr + i)->~T();
	}

	m_allocator.deallocate(arr,m_cap);
	//delete[] reinterpret_cast<char*>(arr);
}

template<typename T, typename Alloc>
void Vector<T, Alloc>::push_back(const T& value)
{
	if (m_size == m_cap) reserve(m_cap * 2);
	m_allocator.construct(arr + m_size, value);
	//new (arr + m_size) T(value);
	++m_size;
}

template<typename T, typename Alloc>
void Vector<T, Alloc>::pop_back()
{
	--m_size;
	m_allocator.destroy(arr + m_size);
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
			//std::uninitialized_copy(arr, arr + m_size, temp_arr);
			m_allocator.construct(temp_arr + pos, arr[pos]);
			//new (temp_arr + pos) T(arr[pos]);
		}
	}
	catch (...)
	{
		//Очистка выделенной памяти.
		for (size_t i{ 0 }; i < pos; ++i)
		{
			m_allocator.destroy(temp_arr + i);
			//(temp_arr + pos)->~T();
		}

		m_allocator.deallocate(temp_arr,n);
		//delete[] reinterpret_cast<char*>(temp_arr);
		throw;
	}
	for (size_t i = 0; i < m_size; ++i)
	{
		m_allocator.destroy(arr + i);
	}
	
	m_allocator.deallocate(arr,m_cap);
	
	arr = temp_arr;
	m_cap = n;
}

template<typename T, typename Alloc>
void Vector<T, Alloc>::resize(const size_t size, const T& value)
{
	if (size > m_cap) reserve(size);
	for (size_t i = m_size; i < size; ++i)
	{
		m_allocator.construct(arr + i, value);
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
T& Vector<T, Alloc>::at(const size_t index)
{
	if (index >= m_size) throw std::out_of_range("out of range");
	return *(arr + index);
}


