#include <iostream>
#include <assert.h>
#include "../src/nd.h"


int main() {
   {
      int size = 20;

      ndarray<bool> e = nd::empty<bool>(size);

      ndarray<float> z = nd::zeros<float>(size);
      for(int i=0; i<size; ++i)
         assert(z[i] == 0);

      ndarray<bool> o = nd::ones<bool>(size);
      for(int i=0; i<size; ++i)
         assert(o[i] == 1);
   }

   {
      const int size = 6;
      typedef int dtype;

      ndarray<dtype> data = nd::arange<dtype>(size);

      assert(data.size() == size);
      assert(data.front() == 0);
      assert(data.back() == size-1);

      assert(nd::max(data) == size-1);
      assert(nd::min(data) == 0);
      assert(nd::amax(data) == size-1);
      assert(nd::amin(data) == 0);

      typedef float newtype;
      ndarray<newtype> data2 = data.as<newtype>();

      assert(data2.size() == size);
      assert(data2.front() == 0);
      assert(data2.back() == size-1);

      int i = 0;
      for(dtype v : data){
         assert(data[i] == v);
         i += 1;
      }
   }

   {
      const int size = 6;
      typedef int dtype;

      ndarray<dtype> data = nd::arange<dtype>(size);

      int shape_new[] = {1, 2, 3};
      data.reshape(3, shape_new);
      std::cout << "[";
      for(int i=0; i < data.dims(); i++)
         std::cout << data.shape()[i] << (i < data.dims()-1 ? ", " : "");
      std::cout << "] - ";
      printf("%s\n", ((std::string)data).c_str());

      data = nd::squeeze(data);

      std::cout << "[";
      for(int i=0; i < data.dims(); i++)
         std::cout << data.shape()[i] << (i < data.dims()-1 ? ", " : "");
      std::cout << "] - ";
      printf("%s\n", ((std::string)data).c_str());

      assert(data.size() == size);
      assert(data.front() == 0);
      assert(data.back() == size-1);
   }

   {
      const int size = 6;
      int shape[] = {2, 3};
      typedef int dtype;

      ndarray<dtype> data = nd::arange<dtype>(size).reshape(2, shape);

      const int value_new = 10;
      int pos_edit[] = {1, 1};
      data[pos_edit] = value_new;
      assert(data[pos_edit] == value_new);

      ndarray<dtype> data_copy = nd::copy(data);
      for(int i=0; i<data.dims(); ++i)
         assert(shape[i] == data_copy.shape()[i]);
      for(int i=0; i<data.size(); ++i)
         assert(data[i] == data_copy[i]);

      int slice_a[] = {0, 0}, slice_b[] = {2, 2};
      ndarray<dtype> slice = data_copy(slice_a, slice_b);
      for(int i=0; i<slice.size(); ++i)
         assert(slice[i] == data_copy[i]);

      for(int i=0; i<slice.size(); ++i){
         slice.data()[i] = i+1;
         assert(slice[i] == data_copy[i]);
         assert(slice[i] != data[i]);
      }
   }

   {
      const int size = 3;
      typedef int dtype;

      dtype data_data[] = {1, 1, 2};
      ndarray<dtype> data = ndarray<dtype>(size, data_data);

      bool mask_data[] = {true, true, false};
      ndarray<bool> mask(size, mask_data);

      ndarray<dtype> masked = nd::mask(data, mask);
      assert(masked.size() == 2);
      assert(masked[0] == 1);
      assert(masked[1] == 1);

      ndarray<dtype> uniques = nd::unique(data);
      assert(uniques.size() == 2);
      assert(uniques[0] == 1);
      assert(uniques[1] == 2);

      dtype value_t = 12, value_f = 4;
      ndarray<dtype> whered = nd::where<dtype>(mask, value_t, value_f);

      dtype expected[] = {12, 12, 4};
      assert(whered.size() == size);
      for(int i=0; i<size; ++i)
         assert(whered[i] == expected[i]);
   }

   {
      const int size = 4;
      int shape[] = {2, 2};
      typedef int dtype;

      ndarray<dtype> data = nd::arange<dtype>(size).reshape(2, shape);

      ndarray<dtype> data_t = nd::transpose(data);

      int pos_a[2], pos_b[2];
      for(int x=0; x<shape[1]; ++x){
         for(int y=0; y<shape[0]; ++y){
            pos_a[0] = pos_b[1] = x;
            pos_a[1] = pos_b[0] = y;
            assert(data[pos_a] == data_t[pos_b]);
         }
      }
   }

   {
      const int size = 4;
      typedef int dtype;

      ndarray<dtype> data_a = nd::arange<dtype>(size);
      ndarray<dtype> data_b = nd::arange<dtype>(size);
      ndarray<dtype> output(size);

      output = nd::sub<dtype>(data_a, data_b);
      for(int i=0; i<size; ++i)
         assert(output[i] == 0);

      /*
      output = nd::mod(data_a, data_b).as<dtype>();
      for(int i=0; i<size; ++i)
         assert(output[i] == 0);
      */
   }

   {
      const int size = 3;
      typedef float dtype;

      ndarray<dtype> data_a = nd::arange<dtype>(size);
      ndarray<dtype> data_b = nd::ones<dtype>(size);

      ndarray<bool> output(size);
      bool* expected = new bool[size];

      expected[0] = 0; expected[1] = 0; expected[2] = 1;
      output = nd::gt<dtype>(data_a, data_b);
      for(int i=0; i<size; ++i)
         assert(output[i] == expected[i]);

      expected[0] = 1; expected[1] = 1; expected[2] = 0;
      output = nd::le<dtype>(data_a, data_b);
      for(int i=0; i<size; ++i)
         assert(output[i] == expected[i]);

      delete[] expected;
   }

   {
      const int size = 3;
      typedef float dtype;

      ndarray<dtype> data = nd::arange<dtype>(size);

      ndarray<dtype> output = nd::append(data, data);
      for(int i=0; i<size*2; ++i)
         assert(output[i] == i%size);
   }

   printf("Finished!");

   return 0;
}
