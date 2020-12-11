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
        return _full<T>(size, fill_value);
    }


    template <typename T>
    ndarray<T> zeros(const int& size){
        T* data = _full<T>(size, 0);
        ndarray<T> output(size, data);
        return output;
    }


    template <typename T>
    ndarray<T> zeros(const int& dims, const int*& shape){
        T* data = _full<T>(dims, shape, 0);
        ndarray<T> output(dims, shape, data);
        return output;
    }


    template <typename T>
    ndarray<T> ones(const int& size){
        T* data = _full<T>(size, 1);
        ndarray<T> output(size, data);
        return output;
    }


    template <typename T>
    ndarray<T> ones(const int& dims, const int*& shape){
        T* data = _full<T>(dims, shape, 1);
        ndarray<T> output(dims, shape, data);
        return output;
    }


    template <typename T>
    ndarray<T> empty(const int& size){
        T* data = new T[size];
        ndarray<T> output(size, data);
        return output;
    }


    template <typename T>
    ndarray<T> empty(const int& dims, const int*& shape){
        int size = get_size(dims, shape);
        return empty<T>(size);
    }
}
