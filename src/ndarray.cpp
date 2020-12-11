#include "../src/ndarray.h"

int get_size(const int& dims, const int*& shape){
   int size = (int) (dims > 0);
   for(int i=0; i < dims; i++)
      size *= shape[i];
   return size;
}
