#ifndef H_ND
#define H_ND

#include "../src/ndarray.h"
#include <set>
#include <limits>


namespace nd {
    /* Namespace for ndarray related functions. */
    template <typename T, typename K, typename F, typename O>
    ndarray<O> broadcast(ndarray<T> a, ndarray<K> b){
        /* Apply ufunc op to all pairs (a, b) w/ a in A and b in B. */
        F operation;
        int size_a = a.size(), size_b = b.size(), size_out;
        int dims_a = a.dims(), dims_b = b.dims(), dims_out = 0;
        int* shape_a = a.shape(), * shape_b = b.shape(), *shape_out;
        T* data_a = a.data(); K* data_b = b.data(); O* data_output;

        int dims_lesser = (dims_a < dims_b ? dims_a : dims_b), dims_greater = (dims_a > dims_b ? dims_a : dims_b);
        int shape_temp[dims_greater];
        for(int i=0; i<dims_lesser; ++i){
            int value_a = shape_a[dims_a - i - 1];
            int value_b = shape_b[dims_b - i - 1];

            if(value_a != value_b){
                if(value_a != 1 && value_b != 1)
                    throw std::invalid_argument((char*) "Where a, b dimensions not equal, one of them must be 1.");

                shape_temp[i] = (value_a > value_b ? value_a : value_b);
                dims_out += 1;
            } else shape_temp[i] = 0;
        }
        for(int i=dims_lesser, value; i<dims_greater; ++i){
            value = (i < dims_a ? shape_a[dims_a - i - 1] : shape_b[dims_b - i - 1]);

            if(value > 1){
                shape_temp[i] = value;
                dims_out += 1;
            } else shape_temp[i] = 0;
        }

        if(!dims_out){
            // if dims same or dims same w/ prepended ones
            size_out = size_a;
            dims_out = dims_a;
            shape_out = shape_a;
            data_output = new O[size_out];

            for(int i=0; i < size_out; i++)
                data_output[i] = operation(data_a[i], data_b[i]);
        } else {
            shape_out = new int[dims_out];
            for(int i=0, j=0; i<dims_greater; ++i){
                if(shape_temp[i] != 0){
                    shape_out[j] = shape_temp[i];
                    j += 1;
                }
            }
            size_out = get_size(dims_out, shape_out);
            data_output = new O[size_out];

            // TODO iterate and generate data_output
        }

        ndarray<O> output(dims_out, shape_out, data_output);
        delete[] data_output, shape_out;
        return output;
    }


    // Functional
    template <typename T, typename K, typename O=float>
    ndarray<O> append(ndarray<T> array1, ndarray<K> array2){
        int array1_size = array1.size();
        T* array1_data = array1.data();
        int array2_size = array2.size();
        K* array2_data = array2.data();

        int size_total = array1_size + array2_size;
        O data[size_total];
        std::copy(array1_data, array1_data+array1_size, data);
        std::copy(array2_data, array2_data+array2_size, data+array1_size);
        return ndarray<O>(size_total, data);
    }


    template <typename T>
    ndarray<T> copy(ndarray<T> input){
        /* Return copy of input array. */
        int size_in = input.size();
        int dims_in = input.dims();
        int* shape_in = input.shape();
        T* data_in = input.data();

        int shape_out[dims_in];
        std::copy(shape_in, shape_in+dims_in, shape_out);

        T data_out[size_in];
        std::copy(data_in, data_in+size_in, data_out);
        return ndarray<T>(dims_in, shape_out, data_out);
    }


    template <typename T>
    ndarray<T> squeeze(ndarray<T> input){
        /* Return version of input without any single valued dimensions. */
        int dims_old = input.dims();
        int* shape_old = input.shape();

        int dims_new = dims_old;
        bool mask[dims_old];
        for(int i=0; i < dims_old; i++){
            bool invalid = shape_old[i] == 1;

            dims_new -= invalid;
            mask[i] = !invalid;
        }

        int shape_new[dims_new];
        for(int i=0, j=0; i < dims_old; i++){
            if(mask[i]){
                shape_new[j] = shape_old[i];
                j += 1;
            }
        }
        
        int size_in = input.size();
        T* data_in = input.data();
        T data_out[size_in];
        std::copy(data_in, data_in+size_in, data_out);
        return ndarray<T>(dims_new, shape_new, data_out);
    }

    // Logical
    template <typename T>
    ndarray<T> mask(ndarray<T>& array, ndarray<bool> mask){
        /* Return flatten set of values from array specified in mask. */
        T* data_in = array.data();
        bool* data_mask = mask.data();

        T data_temp[array.size()];
        int j = 0;
        for(int i=0; i<mask.size(); i++){
            if(data_mask[i]){
                data_temp[j] = data_in[i];
                j += 1;
            }
        }

        T data_out[j];
        std::copy(data_temp, data_temp + j, data_out);
        return ndarray<T>(j, data_out);
    }

    template <typename T, typename K, typename O>
    struct _add_operator{ O operator()(T a, K b) const{ return a + b; } };
    template <typename T, typename K, typename O>
    struct _sub_operator{ O operator()(T a, K b) const{ return a - b; } };
    template <typename T, typename K, typename O>
    struct _mul_operator{ O operator()(T a, K b) const{ return a * b; } };
    template <typename T, typename K, typename O>
    struct _div_operator{ O operator()(T a, K b) const{ return a / b; } };
    template <typename T, typename K, typename O>
    struct _mod_operator{ O operator()(T a, K b) const{ return a % b; } };

    template <typename T, typename K, typename O=float>
    ndarray<O> add(ndarray<T> a, ndarray<K> b){ return broadcast<T, K, _add_operator<T, K, O>, O>(a, b); }
    template <typename T, typename K, typename O=float>
    ndarray<O> sub(ndarray<T> a, ndarray<K> b){ return broadcast<T, K, _sub_operator<T, K, O>, O>(a, b); }
    template <typename T, typename K, typename O=float>
    ndarray<O> mul(ndarray<T> a, ndarray<K> b){ return broadcast<T, K, _mul_operator<T, K, O>, O>(a, b); }
    template <typename T, typename K, typename O=float>
    ndarray<O> div(ndarray<T> a, ndarray<K> b){ return broadcast<T, K, _div_operator<T, K, O>, O>(a, b); }
    template <typename T, typename K, typename O=float>
    ndarray<O> mod(ndarray<T> a, ndarray<K> b){ return broadcast<T, K, _mod_operator<T, K, O>, O>(a, b); }


    template <typename T, typename K>
    struct _gt_operator{ bool operator()(T a, K b) const{ return a > b; } };
    template <typename T, typename K>
    struct _ge_operator{ bool operator()(T a, K b) const{ return a >= b; } };
    template <typename T, typename K>
    struct _lt_operator{ bool operator()(T a, K b) const{ return a < b; } };
    template <typename T, typename K>
    struct _le_operator{ bool operator()(T a, K b) const{ return a <= b; } };
    template <typename T, typename K>
    struct _eq_operator{ bool operator()(T a, K b) const{ return a == b; } };
    template <typename T, typename K>
    struct _ne_operator{ bool operator()(T a, K b) const{ return a != b; } };

    template <typename T, typename K>
    ndarray<bool> gt(ndarray<T> a, ndarray<K> b){ return broadcast<T, K, _gt_operator<T, K>, bool>(a, b); }
    template <typename T, typename K>
    ndarray<bool> ge(ndarray<T> a, ndarray<K> b){ return broadcast<T, K, _ge_operator<T, K>, bool>(a, b); }
    template <typename T, typename K>
    ndarray<bool> lt(ndarray<T> a, ndarray<K> b){ return broadcast<T, K, _lt_operator<T, K>, bool>(a, b); }
    template <typename T, typename K>
    ndarray<bool> le(ndarray<T> a, ndarray<K> b){ return broadcast<T, K, _le_operator<T, K>, bool>(a, b); }
    template <typename T, typename K>
    ndarray<bool> eq(ndarray<T> a, ndarray<K> b){ return broadcast<T, K, _eq_operator<T, K>, bool>(a, b); }
    template <typename T, typename K>
    ndarray<bool> ne(ndarray<T> a, ndarray<K> b){ return broadcast<T, K, _ne_operator<T, K>, bool>(a, b); }


    template <typename T>
    struct _min_operator{ bool operator()(T a, T b) const{ return a < b; } };
    template <typename T>
    struct _max_operator{ bool operator()(T a, T b) const{ return a > b; } };

    template <typename T, typename C>
    T compare_value(ndarray<T> input){
        C comparator;
        T value_min = std::numeric_limits<T>::min(), value_max = std::numeric_limits<T>::max();
        T value, value_curr = (comparator(value_min, value_max) ? value_max : value_min);

        for(int i=0; i < input.size(); i++){
            value = input.data()[i];
            if (comparator(value, value_curr))
                value_curr = value;
        }

        return value_curr;
    }
    template <typename T, typename C>
    T* compare_value(ndarray<T> input, int axis){
        int dims_in = input.dims();
        int* shape_in = input.shape();
        T* data_in = input.data();

        if(abs(axis) > dims_in || axis == dims_in)
            throw std::invalid_argument((char*) "abs(axis) cannot be greater than dims!");
        if(axis < 0)
            axis = dims_in + axis;
        int axis_size = shape_in[axis];

        C comparator;
        T value_min = std::numeric_limits<T>::min(), value_max = std::numeric_limits<T>::max();
        T value = (comparator(value_min, value_max) ? value_max : value_min), * value_curr = new T[axis_size];
        for(int i=0; i<axis_size; ++i)
            value_curr[i] = value;

        for(int i=0, j, *pos; i < input.size(); ++i){
            value = data_in[i];
            pos = get_pos(dims_in, shape_in, i);
            j = pos[axis];
            delete[] pos;
            if (comparator(value, value_curr[j]))
                value_curr[j] = value;
        }

        return value_curr;
    }
    template <typename T>
    T min(ndarray<T> input){ return compare_value<T, _min_operator<T>>(input); }
    template <typename T>
    T* min(ndarray<T> input, int axis){ return compare_value<T, _min_operator<T>>(input, axis); }
    template <typename T>
    T max(ndarray<T> input){ return compare_value<T, _max_operator<T>>(input); }
    template <typename T>
    T* max(ndarray<T> input, int axis){ return compare_value<T, _max_operator<T>>(input, axis); }

    template <typename T, typename C>
    int compare_idx(ndarray<T> input){
        C comparator;
        T value_min = std::numeric_limits<T>::min(), value_max = std::numeric_limits<T>::max();
        T value, value_curr = (comparator(value_min, value_max) ? value_max : value_min);
        int idx_curr = -1;

        for(int i=0; i < input.size(); i++){
            value = input.data()[i];
            if (comparator(value, value_curr)){
                value_curr = value;
                idx_curr = i;
            }
        }

        return idx_curr;
    }
    template <typename T, typename C>
    int* compare_idx(ndarray<T> input, int axis){
        int dims_in = input.dims();
        int* shape_in = input.shape();
        T* data_in = input.data();

        if(abs(axis) > dims_in || axis == dims_in)
            throw std::invalid_argument((char*) "abs(axis) cannot be greater than dims!");
        if(axis < 0)
            axis = dims_in + axis;
        int axis_size = shape_in[axis];

        C comparator;
        T value_min = std::numeric_limits<T>::min(), value_max = std::numeric_limits<T>::max();
        T value = (comparator(value_min, value_max) ? value_max : value_min), * value_curr = new T[axis_size];
        int* idx_curr = new int[axis_size];
        for(int i=0; i<axis_size; ++i){
            idx_curr[i] = -1;
            value_curr[i] = value;
        }

        for(int i=0, j, *pos; i < input.size(); i++){
            value = data_in[i];
            pos = get_pos(dims_in, shape_in, i);
            j = pos[axis];
            delete[] pos;
            if (comparator(value, value_curr[j])){
                value_curr[j] = value;
                idx_curr[j] = i;
            }
        }

        delete[] value_curr;
        return idx_curr;
    }
    template <typename T>
    int amin(ndarray<T> input){ return compare_idx<T, _min_operator<T>>(input); }
    template <typename T>
    int* amin(ndarray<T> input, int axis){ return compare_idx<T, _min_operator<T>>(input, axis); }
    template <typename T>
    int amax(ndarray<T> input){ return compare_idx<T, _max_operator<T>>(input); }
    template <typename T>
    int* amax(ndarray<T> input, int axis){ return compare_idx<T, _max_operator<T>>(input, axis); }


    template <typename T>
    ndarray<T> transpose(ndarray<T> input){
        /* Return transpose of input - across last 2 dimensions. */
        int size = input.size();
        int dims = input.dims();
        int* shape = input.shape();

        if(dims < 2)
            throw std::invalid_argument("Must be at least 2 dimensions to transpose.");
        if(shape[dims-1] != shape[dims-2])
            throw std::invalid_argument("Last 2 dimensions must be square to transpose.");

        T data_out[size];
        ndarray<T> output(dims, shape, data_out);

        T value;
        int temp, dim_m1 = dims-1, dim_m2 = dims-2;
        int* pos;
        for(int i=0; i<size; ++i){
            pos = get_pos(dims, shape, i);

            value = input[pos];

            temp = pos[dim_m1];
            pos[dim_m1] = pos[dim_m2];
            pos[dim_m2] = temp;

            output[pos] = value;
            delete[] pos;
        }

        return output;
    }


    template <typename T>
    ndarray<T> unique(ndarray<T> input){
        /* Return ndarray of unique items. */
        int size_in = input.size();
        T* data_in = input.data();

        std::set<T> u;
        T data_temp[size_in];
        std::pair<std::set<int>::iterator, bool> ret;
        for(int i=0, j=0; i < size_in; i++){
            ret = u.insert(data_in[i]);

            if(ret.second){
                data_temp[j] = data_in[i];
                j += 1;
            }
        }

        int size_out = u.size();
        T data_out[size_out];
        std::copy(data_temp, data_temp+size_out, data_out);
        return ndarray<T>(size_out, data_out);
    }


    template <typename T>
    ndarray<T> where(ndarray<bool> mask, const T& fill_true, const T& fill_false){
        int size_output = mask.size();
        bool* data_input = mask.data();
        T data_output[size_output];

        for(int i=0; i < size_output; i++)
            data_output[i] = (data_input[i] ? fill_true : fill_false);

        return ndarray<T>(mask.dims(), mask.shape(), data_output);
    }

    // Creation
    template <typename T>
    T* full(const int& size, const T& fill_value){
        T* data = new T[size];
        for(int i=0; i < size; i++){
            data[i] = fill_value;
        }
        return data;
    }


    template <typename T>
    T* full(const int& dims, const int* shape, const T& fill_value){
        int size = get_size(dims, shape);
        return full<T>(size, fill_value);
    }


    template <typename T>
    ndarray<T> zeros(const int& size){
        T* data = full<T>(size, 0);
        ndarray<T> output(size, data);
        delete[] data;
        return output;
    }


    template <typename T>
    ndarray<T> zeros(const int& dims, const int* shape){
        T* data = full<T>(dims, shape, 0);
        ndarray<T> output(dims, shape, data);
        delete[] data;
        return output;
    }


    template <typename T>
    ndarray<T> ones(const int& size){
        T* data = full<T>(size, 1);
        ndarray<T> output(size, data);
        delete[] data;
        return output;
    }


    template <typename T>
    ndarray<T> ones(const int& dims, const int* shape){
        T* data = full<T>(dims, shape, 1);
        ndarray<T> output(dims, shape, data);
        delete[] data;
        return output;
    }


    template <typename T>
    ndarray<T> empty(const int& size){
        ndarray<T> output(size);
        return output;
    }


    template <typename T>
    ndarray<T> empty(const int& dims, const int* shape){
        int size = get_size(dims, shape);
        return empty<T>(size).reshape(dims, shape);
    }

    template <typename T>
    ndarray<T> arange(T start, T stop, T step=1){
        int size = int((stop - start) / step);
        T* data = new T[size];
        for(int i=0; i < size; i++)
            data[i] = start + (i * step);

        ndarray<T> output(size, data);
        delete[] data;
        return output;
    }
    template <typename T>
    ndarray<T> arange(T stop){
        T zero = 0;
        return arange(zero, stop);
    }
}


# endif
