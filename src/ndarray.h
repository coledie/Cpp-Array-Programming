#ifndef H_NDARRAY
#define H_NDARRAY

#include "../src/nditerator.h"
#include <stdexcept>
#include <cmath>


int get_size(const int&, const int*);
int get_idx(const int&, const int*, const int*);
int* get_pos(const int&, const int*, const int&);

template <typename T>
class ndarray {
   /* N Dimensional array. */
      int _size;
      int _dims;
      int* _shape;
      T* _data;

   public:
      ndarray(const int& __size){
         _size = __size;
         _dims = 1;
         _shape = new int[_dims]; _shape[0] = _size;
         _data = new T[_size];
      }

      ndarray(const int& __size, T* __data, bool reference=false){
         _size = __size;
         _dims = 1;
         _shape = new int[_dims]; _shape[0] = _size;
         if(reference){
            _data = __data;
         } else {
            _data = new T[_size]; std::copy(__data, __data+_size, _data);
         }
      }

      ndarray(const int& __dims, const int* __shape, T* __data, bool reference=false){
         _dims = __dims;
         _shape = new int[_dims]; std::copy(__shape, __shape+_dims, _shape);
         _size = get_size(__dims, __shape);
         if(reference){
            _data = __data;
         } else {
            _data = new T[_size]; std::copy(__data, __data+_size, _data);
         }
      }

      ndarray(const ndarray<T>& old){
         _size = old.size();
         _dims = old.dims();
         _shape = new int[_dims]; std::copy(old.shape(), old.shape()+_dims, _shape);
         _data = new T[_size]; std::copy(old.data(), old.data()+_size, _data);
      }

      ~ndarray(){
         delete[] _shape;
         delete[] _data;
      }

      typedef ndarray_iterator<T> iterator; 
      iterator begin(){ return iterator(data(), 0); }
      iterator end(){ return iterator(data(), _size); }

      int size() const{ return _size; }
      int dims() const{ return _dims; }
      int* shape() const{ return _shape; }
      T* data() const{ return _data; }

      T front() const{ return _data[0]; }
      T back() const{ return _data[_size-1]; }

      template <typename K>
      ndarray<K> as() const{
         /* Using data in this array, return similar array of type K. */
         K *output = new K[_size];

         for(int i=0; i < _size; i++){
            output[i] = (K) _data[i];
         }

         ndarray<K>output_nd(_size, output);
         delete[] output;
         return output_nd;
      };

      ndarray<T>& reshape(const int& dims_new, const int* shape_new){
         /* Update shape of this array and return it. */
         int size_new = get_size(dims_new, shape_new);
         if(size_new != _size)
            throw std::invalid_argument((char*) ("Reshaped array must be same size as previous."));
         
         _dims = dims_new;
         delete[] _shape;
         _shape = new int[_dims];
         for(int i=0; i<_dims; ++i)
            _shape[i] = shape_new[i];

         return *this;
      }

      ndarray<T>& operator=(ndarray<T> other){
         delete[] _shape;
         delete[] _data;

         _size = other.size();
         _dims = other.dims();
         _shape = new int[_dims]; std::copy(other.shape(), other.shape()+_dims, _shape);
         _data = new T[_size]; std::copy(other.data(), other.data()+_size, _data);

         return *this;
      }

      operator char*() const {
         /* Printf usage. */
         std::string output = "";
         for(int i=0; i<_dims; ++i)
            output += "[";

         int pos_old[_dims] = {0}, *pos, n_changes;
         for(int i=0; i<_size; ++i){
            pos = get_pos(_dims, _shape, i);

            n_changes = 0;
            for(int j=0; j<_dims-1; ++j)
               if(pos[j] != pos_old[j])
                  n_changes += 1;
            for(int j=0; j<n_changes; ++j)
               output += "]";
            if(n_changes)
               output += ", ";
            for(int j=0; j<n_changes; ++j)
               output += "[";

            output += std::to_string(_data[i]);
            output += (pos[_dims-1] < _shape[_dims-1]-1 ? ", " : "");

            std::copy(pos, pos + _dims, pos_old);
            delete[] pos;
         }

         for(int j=0; j<_dims; ++j)
            output += "]";

         char* char_out = new char[output.size()+1];
         output.copy(char_out, output.size()+1);
         char_out[output.size()] = '\0';
         return char_out;
      }

      T& operator[](int idx) const{
         /* Indexing operator */
         if(abs(idx) > _size || idx == _size)
            throw std::invalid_argument((char*) "abs(idx) cannot be greater than size!");
         if(idx < 0)
            idx = _size - idx;
         return _data[idx];
      }
      T& operator[](const int* pos) const{
         /* Indexing operator */
         int* pos_real = new int[_dims];
         for(int i=0; i<_dims; ++i){
            int idx = pos[i];

            if(abs(idx) > _size || idx == _size)
               throw std::invalid_argument((char*) "abs(idx) cannot be greater than size!");

            pos_real[i] = (idx >= 0 ? idx : _size - idx);
         }
         return _data[get_idx(_dims, _shape, pos)];
      }

      ndarray<T> operator()(int idx1, int idx2) const{
         /* Slicing operator. */
         if(abs(idx1) > _size || idx1 == _size)
            throw std::invalid_argument((char*) "abs(idx1) cannot be greater than size!");
         if(abs(idx2) > _size || idx2 == _size)
            throw std::invalid_argument((char*) "abs(idx2) cannot be greater than size!");
         if(idx1 < 0)
            idx1 = _size - idx1;
         if(idx2 < 0)
            idx2 = _size - idx2;

         int size_out = idx2 - idx1;

         return ndarray<T>(size_out, _data+idx1, true);
      }
      ndarray<T> operator()(const int* pos1, const int* pos2) const{
         /* Slicing operator. */
         int* shape_out = new int[_dims], * pos1_real = new int[_dims], * pos2_real = new int[_dims];
         for(int i=0; i<_dims; ++i){
            int idx1 = pos1[i], idx2 = pos2[i];

            if(abs(idx1) > _size || idx1 == _size)
               throw std::invalid_argument((char*) "abs(idx2) cannot be greater than size!");
            if(abs(idx2) > _size || idx2 == _size)
               throw std::invalid_argument((char*) "abs(idx2) cannot be greater than size!");

            pos1_real[i] = (idx1 >= 0 ? idx1 : _size - idx1);
            pos2_real[i] = (idx2 >= 0 ? idx2 : _size - idx2);

            shape_out[i] = pos2_real[i] - pos1_real[i];
         }

         int idx_initial = get_idx(_dims, _shape, pos1_real);

         return ndarray<T>(_dims, shape_out, _data+idx_initial, true);
      }
};


#endif
