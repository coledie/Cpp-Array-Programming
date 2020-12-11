#ifndef H_NDARRAY
#define H_NDARRAY


template <typename T>
class ndarray {
      int size;
      int dims;
      int *shape;
      T *data;

   public:
      ndarray(const int);
      ndarray(const int, T*);
      ndarray(const int, const int*);
      ndarray(const int, const int*, T*);
      ~ndarray();

      operator char*() const {
         // for use in printf
         return (char*)"Not implemented.";
      }

      template <typename K>
      ndarray<K> as();
};


template <typename T>
ndarray<T>::ndarray(const int _size){
   size = _size;
   dims = 1;
   shape = new int[dims]; shape[0] = size;
   data = new T[size];
}


template <typename T>
ndarray<T>::ndarray(const int _size, T* _data){
   size = _size;
   dims = 1;
   shape = new int[dims]; shape[0] = size;
   data = _data;
}


template <typename T>
ndarray<T>::ndarray(const int _dims, const int* _shape){
   dims = _dims;
   shape = _shape;
   size = 1; for(int i=0; i<_dims; i++) size *= shape[i];
   data = new T[size];
}


template <typename T>
ndarray<T>::ndarray(const int _dims, const int* _shape, T* _data){
   dims = _dims;
   shape = _shape;
   size = 1; for(int i=0; i<_dims; i++) size *= shape[i];
   data = _data;
}


template <typename T>
ndarray<T>::~ndarray(){
   delete [] data;
}


template <typename T>
template <typename K>
ndarray<K> ndarray<T>::as(){
   K *output = new K[size];

   for(int i=0; i < size; i++){
      output[i] = (K) data[i];
   }

   return ndarray<K>(size, output);
};


#endif
