#pragma once

#include <cstddef>
#include <utility>

namespace smart_ptr {

template <typename T>
class shared_ptr
{
private:
	class ptr_data
	{
    private:
		class ref_count
		{
	    public:
			ref_count() 
				: m_rc(1) 
			{
			}

		public:
			bool dec()
			{
				if(m_rc == 0)
					return false;
				--m_rc;
				return true;
			}

			void inc()
			{
				++m_rc;
			}

			int use_count() const
			{
				return m_rc;
			}

		private:
			int m_rc;
		};

	public:
		ptr_data(T* value)
			: m_value(value)
		{
		}

		ptr_data(const ptr_data& other)
			: m_value(other.m_value)
			, m_ref_count(other.m_ref_count)
		{	
		}

		ptr_data& operator=(const ptr_data& other)
		{
			m_ref_count = other.m_ref_count;
			m_value = other.m_value;
			//T* p_m_value = m_value;
			//m_value = new T(*other.m_value);
			//delete p_m_value;
			return *this;
		}

	public:
		bool operator==(const ptr_data& other) const
		{
			return (m_ref_count == other.m_ref_count &&
					m_value == other.m_value);
		}

		bool operator!=(const ptr_data& other) const
		{
			return !(*this == other);
		}

	public:
		void inc()
		{
			return m_ref_count.inc();
		}

		bool dec()
		{
			return m_ref_count.dec();
		}

		T* get()
		{
			return  m_value;
		}

		const T* get() const
		{
			return m_value;
		}

		T& value()
		{
			return *m_value;
		}

		const T& value() const
		{
			return *m_value;
		}

		int use_count() const
		{
			return m_ref_count.use_count();
		}

	private:
		T* m_value;
		ref_count m_ref_count;
	};

public:
	shared_ptr();
	shared_ptr(T* data);

	shared_ptr(const shared_ptr& other);
	shared_ptr(shared_ptr&& other);

	~shared_ptr();

public:
	shared_ptr& operator=(const shared_ptr& other);
	shared_ptr& operator=(shared_ptr&& other);
	//TODO: add move assignment operator

public:
	T* get() const;
	void swap(shared_ptr& other);
	void reset();
	void reset(T* ptr);
public:
	T& operator*() const;
	T* operator->() const;

	bool operator==(const shared_ptr& other) const;
	bool operator==(std::nullptr_t) const;
	bool operator!=(std::nullptr_t) const;
	bool operator!=(const shared_ptr& other) const;
	bool operator< (const shared_ptr& other) const;
	bool operator<=(const shared_ptr& other) const;
	bool operator> (const shared_ptr& other) const;
	bool operator>=(const shared_ptr& other) const;
	operator bool() const;

public:
	int use_count() const
	{
		if (!m_data) {
			return 0;
		}
		return m_data->use_count();
	}

private:
	ptr_data* m_data;
};

template <typename T>
shared_ptr<T>::shared_ptr()
	: m_data(nullptr)
{
}

template <typename T>
shared_ptr<T>::shared_ptr(T* data)
	: m_data(new ptr_data(data))
{
}

template <typename T>
shared_ptr<T>::shared_ptr(const shared_ptr& other)
	: m_data(other.m_data)
{
	if (m_data) {
		m_data->inc();
	}
}

template <typename T>
shared_ptr<T>::shared_ptr(shared_ptr&& other)
	: m_data(other.m_data)
{
	other.m_data = nullptr;
}

template <typename T>
shared_ptr<T>::~shared_ptr()
{
	if (m_data && !m_data->dec()) {
		delete m_data;
		m_data = nullptr;
	}
}

template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr& other)
{
	if (m_data && !m_data->dec()) {
		delete m_data;
		m_data = nullptr;
	}
	m_data = other.m_data;
	m_data->inc();
	//ptr_data* temp_ptr = m_data;
	//m_data = new ptr_data(other.m_data);
	//delete temp_ptr;
	return *this;
}

template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr&& other)
{
	m_data = std::move(other.m_data);
	other.m_data = nullptr;
	return *this;
}

template <typename T>
T* shared_ptr<T>::get() const
{
	if (!m_data) {
		return nullptr;
	}
	return m_data->get();
}

template <typename T>
void shared_ptr<T>::swap(shared_ptr& other)
{
	std::swap(m_data, other.m_data);
}

template <typename T>
void shared_ptr<T>::reset()
{
	if(m_data){
		m_data->dec();
		delete m_data;
		m_data = nullptr;
	}
}

template <typename T>
void shared_ptr<T>::reset(T* ptr)
{
	m_data->dec();
	m_data = new ptr_data(ptr);
}

template <typename T>
T& shared_ptr<T>::operator*() const
{
	return m_data->value();
}

template <typename T>
T* shared_ptr<T>::operator->() const
{
	return get();
}

template <typename T>
bool shared_ptr<T>::operator==(const shared_ptr<T>& other) const
{
	return m_data == other.m_data;
}

template <typename T>
bool shared_ptr<T>::operator==(std::nullptr_t nullp) const
{
	return m_data == nullptr;
}

template <typename T>
bool shared_ptr<T>::operator!=(const shared_ptr<T>& other) const
{
	return m_data != other.m_data;
}

template <typename T>
bool shared_ptr<T>::operator!=(std::nullptr_t nullp) const
{
	return m_data != nullptr;
}

template <typename T>
bool shared_ptr<T>::operator>(const shared_ptr& other) const
{
	return m_data > other.m_data;
}

template <typename T>
bool shared_ptr<T>::operator>=(const shared_ptr& other) const
{
	return m_data >= other.m_data;
}

template <typename T>
bool shared_ptr<T>::operator<(const shared_ptr& other) const
{
	return m_data < other.m_data;
}

template <typename T>
bool shared_ptr<T>::operator<=(const shared_ptr& other) const
{
	return m_data <= other.m_data;
}

template <typename T>
shared_ptr<T>::operator bool() const
{
	return m_data != nullptr;
}

} // namespace smart_ptr

