#include <iostream>
#include "../src/ndarray.hpp"


int main() {
   ndarray<int> ww(5);

   printf(ww.as<float>());

   return 0;
}
