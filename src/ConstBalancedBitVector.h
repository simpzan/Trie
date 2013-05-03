#ifndef CONST_BALANCED_BIT_VECTOR_H
#define CONST_BALANCED_BIT_VECTOR_H

#include "ConstBitVector.h"
#include "Vector.h"

class ConstBalancedBitVector : public ConstBitVector {
  public:
	ConstBalancedBitVector() {}
	virtual ~ConstBalancedBitVector() {}

	uint64_t findClose(uint64_t id) const;

	virtual void read(std::istream &is);
	virtual void clear();
	virtual void display(std::ostream &os) const;

  private:
	ConstBitVector _is_fars;
	ConstBitVector _is_pioneers;
	Vector<uint32_t> _pioneers;
};

#endif

