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

   ndarray<dtype> data_new = nd::copy(data);
   // EDIT check see if affects other

   return 0;
}
