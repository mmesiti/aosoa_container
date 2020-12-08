#ifndef __ARRAYS_H_
#define __ARRAYS_H_
#include <cassert>

template <typename Array> struct AoSoAElement;

template <int VectorLenghtBytes, int NComponents, class DataType> struct AoSoA {

  DataType *ptr;
  using DType = DataType;

  static const int N = NComponents;
  static const int NVec = VectorLenghtBytes / sizeof(DataType);
  static const int BlockSize = NVec * NComponents;

  AoSoA(void *p, int mem_size_to_check) {
    ptr = (DataType *)p;
    assert(mem_size_to_check % BlockSize == 0);
  }

  using AoSoAType = AoSoA<VectorLenghtBytes, NComponents, DataType>;

  inline AoSoAElement<AoSoAType> operator[](int idx) {

    return AoSoAElement<AoSoAType>(*this, idx);
  }

  inline DataType &get(int idx, int icomponent) {

    const int iblock = idx / NVec;
    const int in_block_offset = idx % NVec;
    const int address =
        BlockSize * iblock + NVec * icomponent + in_block_offset;

    return ptr[address];
  }
};
/* Auxiliary class for accessing single structure lazily */
template <typename Array> struct AoSoAElement {
  Array &array;
  int idx;
  AoSoAElement(Array &_array, int _idx) : array(_array), idx(_idx) {}

  inline typename Array::DType &operator[](int icomponent) {
    return array.get(idx, icomponent);
  }
  inline typename Array::DType operator()(int icomponent) const {
    return array.get(idx, icomponent);
  }

#define _DECLARE_OPERATOR(OPERATOR)                                            \
  template <typename Other>                                                    \
  inline AoSoAElement<Array> &operator OPERATOR(const Other &other) {          \
    for (int i = 0; i != Array::N; ++i) {                                      \
      (*this)[i] OPERATOR other[i];                                            \
    }                                                                          \
    return (*this);                                                            \
  }                                                                            \
                                                                               \
  inline AoSoAElement<Array> &operator OPERATOR(                               \
      const typename Array::DType scalar) {                                    \
    for (int i = 0; i != Array::N; ++i) {                                      \
      (*this)[i] OPERATOR scalar;                                              \
    }                                                                          \
    return (*this);                                                            \
  }

  _DECLARE_OPERATOR(=);
  _DECLARE_OPERATOR(+=);
  _DECLARE_OPERATOR(-=);
  _DECLARE_OPERATOR(*=);
  _DECLARE_OPERATOR(/=);
#undef _DECLARE_OPERATOR

#define _DECLARE_OPERATOR(OPERATOR)                                            \
  inline AoSoAElement<Array> &operator OPERATOR(                               \
      const AoSoAElement<Array> &other) {                                      \
    for (int i = 0; i != Array::N; ++i) {                                      \
      (*this)[i] OPERATOR other(i);                                            \
    }                                                                          \
    return (*this);                                                            \
  }

  _DECLARE_OPERATOR(=)
  _DECLARE_OPERATOR(*=)
  _DECLARE_OPERATOR(+=)
  _DECLARE_OPERATOR(-=)
  _DECLARE_OPERATOR(/=)
#undef _DECLARE_OPERATOR
};

#endif // __ARRAYS_H_
