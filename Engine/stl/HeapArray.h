#pragma once

#include <new>
#include <string>
#include <cstring>
#include <cstdint>
#include <ostream>
#include <initializer_list>

template <typename T>
class HeapArray {
	private:
		T* buffer = nullptr;

		uint32_t nElements = 0;

	public:
		HeapArray() {}

		HeapArray(const std::initializer_list<T>& _il) : nElements(_il.size()) {
			this->buffer = (T*) std::malloc(this->nElements * sizeof(T));

			uint32_t index = 0;
			for (const T& n : _il)
				this->buffer[index++] = n;
		}

		HeapArray(const uint32_t _nElements) : nElements(_nElements) {
			this->buffer = (T*) std::malloc(this->nElements * sizeof(T));

			std::memset(this->buffer, 0, this->nElements * sizeof(T));
		}

		HeapArray(const HeapArray<T>& _other) : nElements(_other.nElements) {
			this->buffer = (T*) std::malloc(this->nElements * sizeof(T));

			std::memcpy(this->buffer, _other.buffer, this->nElements * sizeof(T));
		}

		void allocate(const uint32_t _nElements) {
			const uint32_t newNElements = this->nElements + _nElements;

			T* newBuffer = (T*) std::malloc(newNElements * sizeof(T));

			std::memcpy(newBuffer,                   this->buffer, this->nElements * sizeof(T));
			std::memset(newBuffer + this->nElements, 0,            _nElements      * sizeof(T));

			delete[] this->buffer;

			this->buffer = newBuffer;
			this->nElements += _nElements;
		}

		T& operator[](const uint32_t _index) const { return this->buffer[_index]; }

		void printToStream(std::ostream& _stream) const {
			_stream << '(';

			for (uint32_t i = 0; i < this->nElements - 1; i++)
				_stream << this->buffer[i];
			
			_stream << this->buffer[this->nElements - 1] << ')';
		}

		uint32_t getLength() const {
			return this->nElements;
		}

		uint64_t getSize() const {
			return this->nElements * sizeof(T);
		}

		T* getBuffer() const {
			return this->buffer;
		}

		void fill(const T& _val) const {
			std::fill_n(this->buffer, this->nElements, _val);
		}

		~HeapArray() { delete[] this->buffer; }
};

template <typename T>
std::ostream& operator<<(std::ostream& _stream, const HeapArray<T>& _vec) {
	_vec.printToStream(_stream);

	return _stream;
}