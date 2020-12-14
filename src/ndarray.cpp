#include "../src/ndarray.h"


int get_size(const int& dims, const int* shape){
   int size = (int) (dims > 0);
   for(int i=0; i < dims; i++)
      size *= shape[i];
   return size;
}


int get_idx(const int& dims, const int* shape, const int* pos){
   int idx = 0;
   for(int i=0; i<dims; ++i)
      idx += pos[i] * (i ? shape[i-1] : 1);
   return idx;
}


int* get_pos(const int& dims, const int*shape, const int& idx){
   int* pos = new int[dims];
   for(int i=0; i<dims; ++i){
      if(i == dims-1){
         pos[i] = idx % shape[i];
      } else {
         int dim_product = 1;
         for(int j=dims-1; j>i; --j)
            dim_product *= shape[j];
         pos[i] = int(idx / dim_product);
      }
   }
   return pos;
}
