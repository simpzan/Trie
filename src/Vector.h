#ifndef VECTOR_H
#define VECTOR_H

#include <vector>
#include <iostream>

template <typename T>
class Vector {
  public:
	Vector() {}
	~Vector() {}

	void append(T &element) {  _elements.push_back(element);  }
	T &operator[](uint64_t index);

	void read(std::istream &is);
	void write(std::ostream &os);
	void clear() {  _elements.clear();  }
	uint64_t size();
	uint64_t count() {  return _elements.size();  }

  private:
	std::vector<T> _elements;
};

template <typename T>
inline void Vector<T>::write(std::ostream &os) {
	uint64_t count = this->count();
	os.write((char *)&count, sizeof(count));
	uint64_t size = sizeof(T) * count;
	os.write((char *)_elements.data(), size);
}

template <typename T>
inline uint64_t Vector<T>::size() {
	uint64_t count = this->count();
	uint64_t size = sizeof(T) * count;
	return size + sizeof(count);
}

template <typename T>
inline T &Vector<T>::operator[](uint64_t index) {
	assert(index < count());
	return _elements[index];
}

template <typename T>
inline void Vector<T>::read(std::istream &is) {
	uint64_t count = 0;
	is.read((char *)&count, sizeof(count));
	_elements.reserve(count);
	uint64_t size = sizeof(T) * count;
	is.read((char *)_elements.data(), size);
}

#endif
