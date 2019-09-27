#pragma once

#include <cstdint>
#include <ostream>
#include <initializer_list>

template <typename T, uint32_t S>
class StackArray {
	private:
		T buff[S];

	public:
		StackArray() {}

		StackArray(const StackArray<T, S>& _other) {
			std::memcpy(this->buff, _other.buff, S * sizeof(T));
		}

		StackArray(const std::initializer_list<T>& _il) {
			if (S <= _il.size()) {
				uint32_t index = 0;
				for (const T& n : _il) {
					if (index + 1 <= _il.size())
						this->buff[index++] = n;
					else
						break;
				}
			}
		}

		T& operator[](const uint32_t _index) { return this->buff[_index]; }

		void printToStream(std::ostream& _stream) const {
			_stream << '(';

			for (uint32_t i = 0; i < S - 1; i++)
				_stream << this->buff[i] << ", ";

			_stream << this->buff[S - 1] << ')';
		}
};

template <typename T, uint32_t S>
std::ostream& operator<<(std::ostream& _stream, const StackArray<T, S>& _stackArray) {
	_stackArray.printToStream(_stream);

	return _stream;
}