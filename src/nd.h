#ifndef H_ND
#define H_ND

#include "../src/ndarray.h"
#include <set>
#include <limits>


namespace nd {
    /* Namespace for ndarray related functions. */
    // Functional
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

    // Logical
    template <typename T>
    bool _min_operator(T& a, T& b){ return a < b; }
    template <typename T>
    bool _max_operator(T& a, T& b){ return a > b; }


    template <typename T, typename C>
    T compare_value(ndarray<T>& input){
        T value_min = std::numeric_limits<T>::min(), value_max = std::numeric_limits<T>::max();
        T value, value_curr = (C(value_min, value_max) ? value_max : value_min);

        for(int i=0; i < input.size(); i++){
            value = input.data()[i];
            if (value < value_curr)
                value_curr = value;
        }

        return value_curr;
    }


    template <typename T>
    T min(ndarray<T>& input){ return compare_value<T, _min_operator>(input); }
    template <typename T>
    T max(ndarray<T>& input){ return compare_value<T, _max_operator>(input); }


    template <typename T, typename C>
    int compare_idx(ndarray<T>& input){
        T value_min = std::numeric_limits<T>::min(), value_max = std::numeric_limits<T>::max();
        T value, value_curr = (C(value_min, value_max) ? value_max : value_min);
        int idx_curr = -1;

        for(int i=0; i < input.size(); i++){
            value = input.data()[i];
            if (value < value_curr){
                value_curr = value;
                idx_curr = i;
            }
        }

        return idx_curr;
    }


    template <typename T>
    int amin(ndarray<T>& input){ return compare_idx<T, _min_operator>(input); }
    template <typename T>
    int amax(ndarray<T>& input){ return compare_idx<T, _max_operator>(input); }


    template <typename T>
    bool _gt_operator(T& a, T& b){ return a > b; }
    template <typename T>
    bool _ge_operator(T& a, T& b){ return a >= b; }
    template <typename T>
    bool _lt_operator(T& a, T& b){ return a < b; }
    template <typename T>
    bool _le_operator(T& a, T& b){ return a <= b; }
    template <typename T>
    bool _eq_operator(T& a, T& b){ return a == b; }
    template <typename T>
    bool _ne_operator(T& a, T& b){ return a != b; }


    template <typename T, typename C>
    ndarray<T> compare(ndarray<T>& a, ndarray<T>& b){
        int size = a.size();
        T* data_a = a.data();
        T* data_b = b.data();
        bool* data_output = new bool[size];

        for(int i=0; i < size; i++)
            data_output[i] = C(data_a[i], data_b[i]);
        
        ndarray<T> output(size, data_output);
        return output;
    }


    template <typename T>
    ndarray<T> gt(ndarray<T>& a, ndarray<T>& b){ return compare<T, _gt_operator>(a, b); }
    template <typename T>
    ndarray<T> ge(ndarray<T>& a, ndarray<T>& b){ return compare<T, _ge_operator>(a, b); }
    template <typename T>
    ndarray<T> lt(ndarray<T>& a, ndarray<T>& b){ return compare<T, _lt_operator>(a, b); }
    template <typename T>
    ndarray<T> le(ndarray<T>& a, ndarray<T>& b){ return compare<T, _le_operator>(a, b); }
    template <typename T>
    ndarray<T> eq(ndarray<T>& a, ndarray<T>& b){ return compare<T, _eq_operator>(a, b); }
    template <typename T>
    ndarray<T> ne(ndarray<T>& a, ndarray<T>& b){ return compare<T, _ne_operator>(a, b); }


    template <typename T>
    ndarray<T> unique(ndarray<T>& input){
        /* Return ndarray of unique items. */
        int size_in = input.size();
        T* data_in = input.data();

        std::set<T> u();
        T* data_temp = new int[size_in];
        std::pair<std::set<int>::iterator, bool> ret;
        for(int i=0, j=0; i < size_in; i++){
            ret = u.insert(data_in[i]);

            if(ret.second){
                data_temp[j] = data_in[i];
                j += 1;
            }
        }

        int size_out = u.size();
        T* data_out = new int[size_out];
        memcpy(data_out, data_temp, size_out * sizeof(T));
        delete[] data_temp, u;

        ndarray<T> output(size_out, data_out);
        return output;
    }


    template <typename T>
    ndarray<T> where(ndarray<bool>& mask, const T& fill_true, const T& fill_false){
        int size_output = mask.size();
        T* data_input = mask.data();
        T* data_output = new T[size_output];

        for(int i=0; i < size_output; i++)
            data_output[i] = (data_input[i] ? fill_true : fill_false);

        ndarray<T> output(mask.dims(), mask.shape(), mask.data());
        return output;
    }

    // Creation
    template <typename T>
    inline T* full(const int& size, const T& fill_value){
        T* data = new T[size];
        for(int i=0; i < size; i++){
            data[i] = fill_value;
        }
        return data;
    }


    template <typename T>
    T* full(const int& dims, const int*& shape, const T& fill_value){
        int size = get_size(dims, shape);
        return full<T>(size, fill_value);
    }


    template <typename T>
    ndarray<T> zeros(const int& size){
        T* data = full<T>(size, 0);
        ndarray<T> output(size, data);
        return output;
    }


    template <typename T>
    ndarray<T> zeros(const int& dims, const int*& shape){
        T* data = full<T>(dims, shape, 0);
        ndarray<T> output(dims, shape, data);
        return output;
    }


    template <typename T>
    ndarray<T> ones(const int size){
        T* data = full<T>(size, 1);
        ndarray<T> output(size, data);
        return output;
    }


    template <typename T>
    ndarray<T> ones(const int& dims, const int*& shape){
        T* data = full<T>(dims, shape, 1);
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


# endif
