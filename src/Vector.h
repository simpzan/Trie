#ifndef VECTOR_H
#define VECTOR_H

#include <cassert>
#include <vector>
#include <iostream>
#include <iterator>

enum DisplayType {
  kBinary,
  kChar,
  kInt,
};

template <typename T>
class Vector {
 public:
  Vector() {}
  ~Vector() {}

  T &operator[](uint64_t index);
  const T &operator[](uint64_t index) const;
  uint64_t count() const {  return _elements.size();  }
  uint64_t size() const;

  void display(std::ostream &os) const;

  void append(T element) {  _elements.push_back(element);  }
  void read(std::istream &is);
  void write(std::ostream &os) const;
  void clear() {  _elements.clear();  }

 private:
  std::vector<T> _elements;
};

template <typename T>
inline T &Vector<T>::operator[](uint64_t index) {
  assert(index < count());
  return _elements[index];
}

template <typename T>
inline const T &Vector<T>::operator[](uint64_t index) const {
  assert(index < count());
  return _elements[index];
}

template <typename T>
inline void Vector<T>::write(std::ostream &os) const {
  uint64_t count = this->count();
  os.write((char *)&count, sizeof(count));
  uint64_t size = sizeof(T) * count;
  os.write((char *)_elements.data(), size);
}

template <typename T>
inline void Vector<T>::display(std::ostream &os) const {
  os << "count: " << count() << std::endl;
  for (int i = 0; i < count(); ++i) {
    os << i << ":" << _elements[i] << " ";
  }
  os << std::endl;
}

template <typename T>
inline uint64_t Vector<T>::size() const {
  uint64_t count = this->count();
  uint64_t size = sizeof(T) * count;
  return size + sizeof(count);
}

template <typename T>
inline void Vector<T>::read(std::istream &is) {
  uint64_t count = 0;
  is.read((char *)&count, sizeof(count));
  _elements.resize(count);
  uint64_t size = sizeof(T) * count;
  is.read((char *)_elements.data(), size);
}

#endif
