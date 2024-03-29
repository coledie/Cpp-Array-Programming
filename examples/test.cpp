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
   }

   {
      const int size = 6;
      int shape[] = {2, 3};
      typedef int dtype;

      ndarray<dtype> data = nd::arange<dtype>(size).reshape(2, shape);

      ndarray<dtype> data_copy = nd::copy(data);
      for(int i=0; i<data.dims(); ++i)
         assert(shape[i] == data_copy.shape()[i]);
      for(int i=0; i<data.size(); ++i)
         assert(data[i] == data_copy[i]);

      int slice_a[] = {0, 0}, slice_b[] = {2, 2};
      ndarray<dtype> slice = data_copy(slice_a, slice_b);
      int pos_curr[2];
      std::cout << "[";
      for(int i=0; i < slice.dims(); i++)
         std::cout << slice.shape()[i] << (i < slice.dims()-1 ? ", " : "");
      std::cout << "]\n";

      printf("Copy: %s\n", ((std::string)data_copy).c_str());
      printf("Slice: %s\n", ((std::string)slice).c_str());

      for(int y=slice_a[1]; y<slice_b[1]; ++y){
         for(int x=slice_a[0]; x<slice_b[0]; ++x){
            pos_curr[0] = x; pos_curr[1] = y;
            assert(slice[pos_curr] == data_copy[pos_curr]);
         }
      }

      /*
      for(int i=0; i<slice.size(); ++i){
         slice.data()[i] = i+1;
         assert(slice[i] == data_copy[i]);
         assert(slice[i] != data[i]);
      }*/
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
      ndarray<dtype> data_b = nd::arange<dtype>(1, size+1);
      ndarray<dtype> output(size);

      output = nd::sub<dtype>(data_a, data_b);
      for(int i=0; i<size; ++i)
         assert(output[i] == -1);

      output = nd::mod<dtype>(data_a, data_b).as<dtype>();
      for(int i=0; i<size; ++i)
         assert(output[i] == i);
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

      ndarray<int> data_a = nd::arange<int>(size);
      ndarray<float> data_b = nd::arange<float>(size);
      
      ndarray<float> output_1 = nd::append(data_a, data_b);
      for(int i=0; i<size*2; ++i)
         assert(output_1[i] == i%size);


      ndarray<float> output_2 = nd::append(data_b, data_a);
      for(int i=0; i<size*2; ++i)
         assert(output_2[i] == i%size);
   }

   {
      const int size = 6;

      typedef float dtype;

      int shape[] = {2, 3};
      ndarray<dtype> data = nd::arange<dtype>(size).reshape(2, shape);

      dtype* result_max = nd::max(data, -1);
      assert(result_max[0] == 3);
      assert(result_max[1] == 4);
      assert(result_max[2] == 5);
      delete[] result_max;

      int* result_amin = nd::amin(data, 0);
      assert(result_amin[0] == 0);
      assert(result_amin[1] == 3);
      delete[] result_amin;
   }

   {  // Broadcast [(1, 1, 3), (3)]
      const int size = 3;
      typedef float dtype;

      int shape_a[] = {1, 1, 3};
      ndarray<dtype> data_a = nd::arange<dtype>(size).reshape(3, shape_a);
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

   {  // Broadcast [(2, 3), (3)]
      const int size_a = 6, size_b = 3;
      int shape_a[] = {2, 3}, shape_b[] = {3};
      int size_out = size_a;
      typedef float dtype;

      ndarray<dtype> data_a = nd::arange<dtype>(size_a).reshape(2, shape_a);
      ndarray<dtype> data_b = nd::ones<dtype>(size_b).reshape(1, shape_b);

      ndarray<bool> output(size_out);

      bool expected1[] = {0, 0, 1, 1, 1, 1};
      output = nd::gt<dtype>(data_a, data_b);
      for(int i=0; i<size_out; ++i)
         assert(output[i] == expected1[i]);

      bool expected2[] = {1, 1, 0, 0, 0, 0};
      output = nd::le<dtype>(data_a, data_b);
      for(int i=0; i<size_out; ++i)
         assert(output[i] == expected2[i]);
   }

   printf("Finished!");

   return 0;
}
