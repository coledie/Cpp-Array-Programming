#ifndef H_NDARRAY
#define H_NDARRAY


int get_size(const int& dims, const int*& shape){
   int size = (int) (dims > 0);
   for(int i=0; i < dims; i++)
      size *= shape[i];
   return size;
}


template <typename T>
class ndarray {
   /* N Dimensional array. */
      int _size;
      int _dims;
      int* _shape;
      T* _data;

   public:
      ndarray(const int&);
      ndarray(const int&, T*);
      ndarray(const int&, const int*&);
      ndarray(const int&, const int*&, T*);
      ~ndarray();

      int size(){ return _size; }
      int dims(){ return _dims; }
      int* shape(){ return _shape; }
      T* data(){ return _data; }

      operator char*() const {
         // for use in printf
         return (char*)"Not implemented.";
      }

      template <typename K> ndarray<K> as();
      ndarray<T>& reshape(const int&, const int*&);
};


template <typename T>
ndarray<T>::ndarray(const int& __size){
   _size = __size;
   _dims = 1;
   _shape = new int[_dims]; _shape[0] = _size;
   _data = new T[_size];
}


template <typename T>
ndarray<T>::ndarray(const int& __size, T* __data){
   _size = __size;
   _dims = 1;
   _shape = new int[_dims]; _shape[0] = _size;
   _data = __data;
}


template <typename T>
ndarray<T>::ndarray(const int& __dims, const int*& __shape){
   _dims = __dims;
   _shape = __shape;
   _size = get_size(__dims, __shape);
   _data = new T[_size];
}


template <typename T>
ndarray<T>::ndarray(const int& __dims, const int*& __shape, T* __data){
   _dims = __dims;
   _shape = __shape;
   _size = get_size(__dims, __shape);
   _data = __data;
}


template <typename T>
ndarray<T>::~ndarray(){
   delete[] _data;
}


template <typename T>
template <typename K>
ndarray<K> ndarray<T>::as(){
   /* Using data in this array, return similar array of type K. */
   K *output = new K[_size];

   for(int i=0; i < _size; i++){
      output[i] = (K) _data[i];
   }

   return ndarray<K>(_size, output);
};


template <typename T>
ndarray<T>& ndarray<T>::reshape(const int& dims_new, const int*& shape_new){
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


#endif
