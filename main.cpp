#include "aosoa.hpp"
#include <cstdlib>
#include <iomanip>
#include <iostream>

int main(int argc, char *argv[]) {

  const int size = 80;
  const int ncomponents = 4;
  using DataType = double;
  const int VectorLengthBytes = 512 / 8;
  const int mem_size = size * ncomponents * sizeof(DataType);
  auto arr = AoSoA<VectorLengthBytes, ncomponents, DataType>(
      std::malloc(mem_size), mem_size);

  std::cout << "Start:\n";
  std::cout << "NVec:" << arr.NVec << std::endl;

  for (int idx = 0; idx < size; ++idx) {
    for (int icomp = 0; icomp < 4; ++icomp)
      arr[idx][icomp] = 4 * idx + icomp;
  }

  arr[1] += (DataType)22;
  arr[2] -= arr[1];
  arr[3] = (DataType)22;

  double p[ncomponents] = {114, 335, 324, 237};

  arr[4] = p;
  arr[5] += p;
  arr[6] -= p;
  arr[7] *= p;
  arr[8] /= p;

  arr[9] = arr[8];
  arr[10] += arr[8];
  arr[11] -= arr[8];
  arr[12] *= arr[8];
  arr[13] /= arr[8];

  for (int iblock = 0; iblock < size / arr.NVec; ++iblock) {
    for (int icomp = 0; icomp < ncomponents; ++icomp) {
      for (int j = 0; j < arr.NVec; ++j) {
        std::cout << std::setw(10)
                  << arr.ptr[((iblock * ncomponents) + icomp) * arr.NVec + j];
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  std::free(arr.ptr);

  return 0;
}
