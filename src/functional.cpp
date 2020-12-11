#include "../src/ndarray.hpp"


namespace nd {
    /* Namespace for ndarray related functions. */
    template <typename T>
    ndarray<T> copy(ndarray<T> input){
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
}