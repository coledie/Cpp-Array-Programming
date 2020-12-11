#include "../src/nd.h"


namespace nd {
    template <typename T>
    T* _full(const int& size, const T& fill_value){
        T* data = new T[size];
        for(int i=0; i < size; i++){
            data[i] = fill_value;
        }
        return data;
    }


    template <typename T>
    T* _full(const int& dims, const int*& shape, const T& fill_value){
        int size = get_size(dims, shape);
        return _full(size, fill_value);
    }


    template <typename T>
    ndarray<T> zeros(const int& size){
        T* data = _full(size, 0);
        ndarray<T> output(size, data);
        return output;
    }


    template <typename T>
    ndarray<T> zeros(const int& dims, const int*& shape){
        T* data = _full(dims, shape, 0);
        ndarray<T> output(dims, shape, data);
        return output;
    }


    template <typename T>
    ndarray<T> ones(const int& size){
        T* data = _full(size, 1);
        ndarray<T> output(size, data);
        return output;
    }


    template <typename T>
    ndarray<T> ones(const int& dims, const int*& shape){
        T* data = _full(dims, shape, 1);
        ndarray<T> output(dims, shape, data);
        return output;
    }
}
