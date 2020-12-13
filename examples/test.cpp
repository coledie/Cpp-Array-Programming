#include <iostream>
#include "../src/nd.h"


int main() {
   typedef int dtype;
   ndarray<dtype> data = nd::arange<dtype>(5);

   std::cout << data.as<float>() << std::endl;
   std::cout << "Size: " << data.size() << std::endl;

   std::cout << "Shape: [";
   for(int i=0; i < data.dims(); i++)
      std::cout << data.shape()[i] << (i < data.dims()-1 ? ", " : "");
   std::cout << "]" << std::endl;

   for(dtype v : data){
      std::cout << v << std::endl;
   }

   return 0;
}
