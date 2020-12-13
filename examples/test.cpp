#include <iostream>
#include <assert.h>
#include "../src/nd.h"


int main() {
   ndarray<float> z = nd::zeros<float>(20);
   ndarray<bool> o = nd::ones<bool>(6);
   ndarray<bool> e = nd::empty<bool>(7);

   typedef int dtype;
   const int size = 6;
   ndarray<dtype> data = nd::arange<dtype>(size);

   assert(data.size() == size);
   assert(data.front() == 0);
   assert(data.back() == size-1);
   assert(nd::max(data) == size-1);
   assert(nd::min(data) == 0);
   assert(nd::amax(data) == size-1);
   assert(nd::amin(data) == 0);
   assert(data[3] == 3);

   std::cout << "Shape: [";
   for(int i=0; i < data.dims(); i++)
      std::cout << data.shape()[i] << (i < data.dims()-1 ? ", " : "");
   std::cout << "]" << std::endl;

   std::cout << "Float: " << data.as<float>() << std::endl;

   for(dtype v : data)
      std::cout << v << std::endl;

   printf("%s\n", (char*)data);

   int shape_new[] = {2, 3};
   data.reshape(2, shape_new);
   std::cout << "Shape: [";
   for(int i=0; i < data.dims(); i++)
      std::cout << data.shape()[i] << (i < data.dims()-1 ? ", " : "");
   std::cout << "]" << std::endl;
   printf("%s\n", (char*)data);
   assert(data.size() == size);
   assert(data.front() == 0);
   assert(data.back() == size-1);

   int new_value = 10;
   int pos_idx_edit[] = {1, 1};
   printf("%i -> ", data[pos_idx_edit]);
   data[pos_idx_edit] = new_value;
   printf("%i\n", data[pos_idx_edit]);
   assert(data[pos_idx_edit] == new_value);

   ndarray<dtype> data_copy = nd::copy(data);
   for(int i=0; i<data.size(); ++i)
      assert(data[i] == data_copy[i]);

   int pos_slice_a[] = {0, 0}, pos_slice_b[] = {2, 2};
   ndarray<dtype> slice = data_copy(pos_slice_a, pos_slice_b);
   for(int i=0; i<slice.size(); ++i)
      assert(slice[i] == data_copy[i]);

   for(int i=0; i<slice.size(); ++i)
      slice.data()[i] = i+1;
   for(int i=0; i<slice.size(); ++i)
      assert(slice[i] == data_copy[i]);
   for(int i=0; i<slice.size(); ++i)
      assert(slice[i] != data[i]);

   int size_new_copy = 6;
   ndarray<dtype> data_copy2 = nd::resize(data_copy, size_new_copy);
   assert(data_copy2.size() == size_new_copy);
   for(int i=0; i<slice.size(); ++i)
      assert(slice[i] == data_copy2[i]);
   data_copy.~ndarray();
   for(int i=0; i<slice.size(); ++i){
      assert(i+1 == data_copy2[i]);
   }

   return 0;
}
