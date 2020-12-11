#ifndef H_NDARRAY
#define H_NDARRAY


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
};


template <typename T>
ndarray<T>::ndarray(const int& __size){
   _size = __size;
   _dims = 1;
   _shape = new int[_dims]; _shape[0] = _size;
   _data = new T[_size];
}


template <typename T>
ndarray<T>::ndarray(const int& __size, T* _data){
   _size = __size;
   _dims = 1;
   _shape = new int[_dims]; _shape[0] = _size;
   _data = _data;
}


template <typename T>
ndarray<T>::ndarray(const int& __dims, const int*& __shape){
   _dims = __dims;
   _shape = __shape;
   _size = 1; for(int i=0; i<_dims; i++) _size *= _shape[i];
   _data = new T[_size];
}


template <typename T>
ndarray<T>::ndarray(const int& __dims, const int*& __shape, T* _data){
   _dims = __dims;
   _shape = __shape;
   _size = 1; for(int i=0; i<_dims; i++) _size *= _shape[i];
   _data = _data;
}


template <typename T>
ndarray<T>::~ndarray(){
   delete[] _data;
}


template <typename T>
template <typename K>
ndarray<K> ndarray<T>::as(){
   K *output = new K[_size];

   for(int i=0; i < _size; i++){
      output[i] = (K) _data[i];
   }

   return ndarray<K>(_size, output);
};


#endif
