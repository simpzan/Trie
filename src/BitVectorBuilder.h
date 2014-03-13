#ifndef BIT_VECTOR_BUILDER_H
#define BIT_VECTOR_BUILDER_H

#include <stdint.h>
#include <sdsl/int_vector.hpp>

class BitVectorBuilder {
 public:
  BitVectorBuilder() : _size(0), _capacity(8), bitVector(_capacity) {}
  ~BitVectorBuilder() {}

  void addBit(bool bit) {
    if (_size == _capacity) {
    	_capacity *= 2;
      bitVector.bit_resize(_capacity);
    }
  	assert(_size < _capacity);
    bitVector[_size] = bit ? 1 : 0;
    ++_size;
  }

  bool save(std::ostream &os) {
  	bitVector.bit_resize(_size);
  	bitVector.serialize(os);
  }
  
  void display() {
  	std::cout << "bit_size:" << _size
  	<< " capacity:" << _capacity 
  	<< "\n" << bitVector << std::endl;
  }

  sdsl::bit_vector &underlyingBV() {
    bitVector.bit_resize(_size);
    _capacity = _size;
    return bitVector;  
  }

private:
  uint64_t _size;
  uint64_t _capacity;
  sdsl::bit_vector bitVector;
};

#endif