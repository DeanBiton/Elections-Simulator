#pragma once
#include <iostream>
using namespace std;

namespace election
{
	template <class T>
	class DynamicArray
	{
	public:
		DynamicArray(int size = 2) : _logicalSize(0), _physicalSize(size), _arr(nullptr)
		{
			if (size <= 0)
				throw std::invalid_argument("dynamic allocation faild");

			_arr = new T[size];
		}
		DynamicArray(const DynamicArray& other) : _arr(nullptr) {
			*this = other;
		}
		
		~DynamicArray() {
			delete[] _arr;
		}

		const DynamicArray& operator=(const DynamicArray& other) {
			if (this != &other) {
				_logicalSize = other._logicalSize;
				_physicalSize = other._physicalSize;
				delete[] _arr;
				_arr = new T[_physicalSize];
				for (int i = 0; i < _logicalSize; i++)
					_arr[i] = other._arr[i];
			}
			return *this;
		}

		const T& operator[](int i) const { return _arr[i]; }
		T& operator[](int i) { return _arr[i]; }

		void push_back(const T& value) {
			if (_logicalSize == _physicalSize)
				resize();
			_arr[_logicalSize++] = value;
		}

		const T& front()    const { return _arr[0]; }
		int      size()     const { return _logicalSize; }
		int      capacity() const { return _physicalSize; }
		bool     empty()    const { return _logicalSize == 0; }
		void     clear() { _logicalSize = 0; }

		class iterator
		{
		private:
			DynamicArray* _da;
			int				_i;
		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using different_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			iterator(DynamicArray& arr, int i) : _da(&arr), _i(i) {}
			iterator(const iterator& other) : _da(other._da), _i(other._i) {}
			
			// const_iterator should also be constructible from regular iterator
			friend class const_iterator;

			int getIndex() const
			{
				return _i;
			}

			const iterator& operator=(const iterator& other) {
				_da = other._da;
				_i = other._i;
				return *this;
			}

			// comparison with another iterator
			bool operator==(const iterator& other) const {
				return (_da == other._da) && (_i == other._i);
			}
			bool operator!=(const iterator& other) const {
				return !(*this == other);
			}

			// smart-pointer iterator methods
			T& operator*() {			
				return _da->_arr[_i];
			}
			T* operator->() {				
				return &_da->_arr[_i];
			}

			// increment-decrement iterator methods
			virtual iterator& operator++() {
				++_i;
				return *this;
			}
			virtual iterator operator++(int) {
				iterator temp(*this);
				++_i;
				return temp;
			}
			virtual iterator& operator--() {
				--_i;
				return *this;
			}
			virtual iterator operator--(int) {
				iterator temp(*this);
				--_i;
				return temp;
			}
		};

		class const_iterator
		{
		private:
			const DynamicArray* _da;
			int				_i;

		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using different_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = const T*;
			using reference = const T&;

			
			const_iterator(const DynamicArray& arr, int i) : _da(&arr), _i(i) {}
			const_iterator(const const_iterator& other) : _da(other._da), _i(other._i) {}
			const_iterator(const iterator& other) : _da(other._da), _i(other._i) {}

			int getIndex() const
			{
				return _i;
			}

			const const_iterator& operator=(const const_iterator& other) {
				_da = other._da;
				_i = other._i;
				return *this;
			}

			const const_iterator& operator=(const iterator& other) {
				_da = other._da;
				_i = other._i;
				return *this;
			}

			bool operator==(const iterator& other) const {
				return (_da == other._da) && (_i == other._i);
			}
			bool operator!=(const iterator& other) const {
				return !(*this == other);
			}
			// comparison with another iterator
			bool operator==(const const_iterator& other) const {
				return (_da == other._da) && (_i == other._i);
			}
			bool operator!=(const const_iterator& other) const {
				return !(*this == other);
			}

			// smart-pointer iterator methods
			const T& operator*() {
				return _da->_arr[_i];
			}
			const T* operator->() {
				return &_da->_arr[_i];
			}

			// increment-decrement iterator methods
			const_iterator& operator++() {
				++_i;
				return *this;
			}
			const_iterator operator++(int) {
				const_iterator temp(*this);
				++_i;
				return temp;
			}
			const_iterator& operator--() {
				--_i;
				return *this;
			}
			const_iterator operator--(int) {
				const_iterator temp(*this);
				--_i;
				return temp;
			}
		};

		class reverse_iterator
		{
		private:
			DynamicArray* _da;
			int				_i;
		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using different_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			friend class iterator;

			reverse_iterator(DynamicArray& arr, int i) : _da(&arr), _i(i) {}
			reverse_iterator(const reverse_iterator& other) : _da(other._da), _i(other._i) {}

			const reverse_iterator& operator=(const reverse_iterator& other) {
				_da = other._da;
				_i = other._i;
				return *this;
			}

			const reverse_iterator& operator=(const iterator& other) {
				_da = other._da;
				_i = other._i;
				return *this;
			}

			// comparison with another iterator
			bool operator==(const reverse_iterator& other) const {
				return (_da == other._da) && (_i == other._i);
			}
			bool operator!=(const reverse_iterator& other) const {
				return !(*this == other);
			}

			// smart-pointer iterator methods
			T& operator*() {
				return _da->_arr[_i];
			}
			T* operator->() {
				return &_da->_arr[_i];
			}

			// increment-decrement iterator methods
			virtual reverse_iterator& operator++() {
				--_i;
				return *this;
			}
			virtual reverse_iterator operator++(int) {
				reverse_iterator temp(*this);
				--_i;
				return temp;
			}
			virtual reverse_iterator& operator--() {
				++_i;
				return *this;
			}
			virtual reverse_iterator operator--(int) {
				reverse_iterator temp(*this);
				++_i;
				return temp;
			}
		};

		void insert(const iterator& pos, const T& val) {
			if (_logicalSize == _physicalSize)
				resize();

			iterator itrEnd = end();
			iterator itrCurrent = itrEnd, itrPrev = --itrEnd;
			while (itrCurrent != pos)
			{
				*itrCurrent = *itrPrev;
				itrCurrent = itrPrev--;
			}

			iterator p = pos;
			*p = val;
			++_logicalSize;
		}

		const iterator erase(const iterator& iter)
		{
			if (_logicalSize == 0 || iter == end())
				return end();
			else
			{
				iterator itrEnd = end();
				iterator itrCurrent = iter;
				iterator itrnext = itrCurrent;
				itrnext++;
				while (itrnext != itrEnd)
				{
					*itrCurrent++ = *itrnext++;
				}

				_logicalSize--;
				iterator p = iter;
				if (p == begin())
					return p;
				else
					return --p;
			}
		}
		const iterator erase(const iterator& first, const iterator& last)
		{
			if (_logicalSize == 0 || first == end() || first == last)
				return end();
			else
			{
				iterator itrEnd = end();
				iterator itrlast = last;
				
				iterator itrCurrent = first;
				int count = 0;
				while (itrlast != itrEnd)
				{
					*itrCurrent++ = *itrlast++;
				}
				while (itrCurrent != itrlast)
				{
					count++;
					itrlast--;
				}

				_logicalSize -= count;
				iterator p = first;
				if (p == begin())
					return p;
				else
					return --p;
			}
		}

		iterator begin() {
			return iterator(*this, 0);
		}
		iterator end() {
			return iterator(*this, _logicalSize);
		}

		const_iterator begin() const {
			return const_iterator(*this, 0);
		}
		const_iterator end() const {
			return const_iterator(*this, _logicalSize);
		}

		const iterator cbegin() const {
			return iterator(*this, 0);
		}
		const iterator cend() const {
			return iterator(*this, _logicalSize);
		}
		
		reverse_iterator rbegin() {
			return reverse_iterator(*this, _logicalSize);
		}
		reverse_iterator rend() {
			return reverse_iterator(*this, 0);
		}

		void print() const {
			for (int i = 0; i < _logicalSize; i++)
				cout << _arr[i] << " ";
			cout << endl;
		}

	private:
		void resize() {
			_physicalSize *= 2;
			T* temp = new T[_physicalSize];
			for (int i = 0; i < _logicalSize; i++)
				temp[i] = _arr[i];

			delete[] _arr;
			_arr = temp;
		}
		
		T* _arr;
		int _logicalSize;
		int _physicalSize;
	};

}
