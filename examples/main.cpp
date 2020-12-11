#include <iostream>
#include "../src/ndarray.hpp"
#include "../src/functional.cpp"


int main() {
   ndarray<int> data(5);

   std::cout << data.as<float>() << std::endl;
   std::cout << "Size: " << data.size() << std::endl;

   std::cout << "Shape: [";
   for(int i=0; i < data.dims(); i++)
      std::cout << data.shape()[i] << (i < data.dims()-1 ? ", " : "");
   std::cout << "]" << std::endl;

   return 0;
}
