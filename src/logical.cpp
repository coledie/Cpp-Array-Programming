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
}
