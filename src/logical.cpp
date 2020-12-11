#include "../src/nd.h"
#include <set>


namespace nd {
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
