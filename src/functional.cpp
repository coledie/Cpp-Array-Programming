#include "../src/nd.h"


namespace nd {
    template <typename T>
    ndarray<T> append(ndarray<T>& array1, ndarray<T>& array2){
        int array1_size = array1.size();
        T* array1_data = array1.data();
        int array2_size = array2.size();
        T* array2_data = array2.data();

        int size_total = array1_size + array2_size;
        T* data = new T[size_total];
        for(int i=0; i < array1_size; i++)
            data[i] = array1_data[i];
        for(int i=0; i < array2_size; i++)
            data[array1_size + i] = array2_data[i];

        ndarray<T> output(size_total, data);
        return output;
    }


    template <typename T>
    ndarray<T> copy(ndarray<T>& input){
        /* Return copy of input array. */
        int size_in = input.size();
        int dims_in = input.dims();
        int* shape_in = input.shape();
        T* data_in = input.data();

        int* shape_out = new T[dims_in];
        for(int i=0; i < dims_in; i++)
            shape_out[i] = shape_in[i];

        T* data_out = new T[size_in];
        for(int i=0; i < size_in; i++)
            data_out[i] = data_in[i];

        ndarray<T> output(dims_in, shape_out, data_out);

        return output;
    }


    template <typename T>
    ndarray<T> resize(ndarray<T>& input, const int& size_new, const T& fill_value){
        /* Return a version of input with the input size given. */
        int size_old = input.size();
        T* data_old = input.data();

        T* data_new = new T[size_new];
        for(int i=0; i < size_new; i++){
            if(i > size_old){
                data_new[i] = fill_value;
            } else {
                data_new[i] = data_old[i];
            }
        }

        ndarray<T> output(size_new, data_new);
        return output;
    }


    template <typename T>
    ndarray<T> squeeze(ndarray<T>& input){
        /* Return version of input without any single valued dimensions. */
        int dims_old = input.dims();
        int* shape_old = input.shape();

        int dims_new = dims_old;
        bool* mask = new bool[dims_old];
        for(int i=0; i < dims_old; i++){
            bool invalid = shape_old[i] == 1;

            dims_new -= invalid;
            mask[i] = !invalid;
        }

        int* shape_new = new int[dims_new];
        for(int i=0, j=0; i < dims_old; i++){
            if(mask[i]){
                shape_new[j] = shape_old[i];
                j += 1;
            }
        }
        
        ndarray<T> output(dims_new, shape_new, input.data());
        return output;
    }
}
