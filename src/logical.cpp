#include "../src/nd.h"
#include <set>
#include <limits>


namespace nd {
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
}
