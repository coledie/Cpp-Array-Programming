#ifndef H_NDARRAY
#define H_NDARRAY

#include <stdexcept>


int get_size(const int& dims, const int*& shape);


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

      ndarray(const int& __size, T* __data){
         _size = __size;
         _dims = 1;
         _shape = new int[_dims]; _shape[0] = _size;
         _data = __data;
      }

      ndarray(const int& __dims, const int*& __shape){
         _dims = __dims;
         _shape = __shape;
         _size = get_size(__dims, __shape);
         _data = new T[_size];
      }

      ndarray(const int& __dims, const int*& __shape, T* __data){
         _dims = __dims;
         _shape = __shape;
         _size = get_size(__dims, __shape);
         _data = __data;
      }

      ~ndarray(){
         delete[] _data;
      }

      template <typename K>
      ndarray<K> as(){
         /* Using data in this array, return similar array of type K. */
         K *output = new K[_size];

         for(int i=0; i < _size; i++){
            output[i] = (K) _data[i];
         }

         return ndarray<K>(_size, output);
      };

      ndarray<T>& reshape(const int& dims_new, const int*& shape_new){
         /* Update shape of this array and return it. */
         int size_new = get_size(dims_new, shape_new);
         if(size_new != size)
            throw std::invalid_argument((char*) ("Reshaped array must be same size as previous: " + size + " != " + size_new));

         _dims = dims_new;
         delete[] _shape;
         _shape = new int[_dims];
         for(int i=0; i < _dims; i++)
            _shape[i] = shape_new[i];

         return *this;
      }

      int size(){ return _size; }
      int dims(){ return _dims; }
      int* shape(){ return _shape; }
      T* data(){ return _data; }

      operator char*() const {
         // for use in printf
         return (char*)"Not implemented.";
      }
};


#endif
