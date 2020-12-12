#ifndef H_NDITERATOR
#define H_NDITERATOR

#include <iterator>


template <typename T>
class ndarray_iterator {
    /* Iterator over given ndarray. *
    https://accu.org/journals/overload/10/50/goodliffe_389/
    */


    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef T value_type;
        typedef T         *pointer;
        typedef const T   *const_pointer;
        typedef T         &reference;
        typedef const T   &const_reference;
        typedef size_t     size_type;
        typedef ptrdiff_t  difference_type;
        typedef T cbuf_type;

        ndarray_iterator(cbuf_type *b, size_type start_pos)
            : buf_(b), pos_(start_pos) {}

        T &operator*(){ return buf_[pos_]; }
        T *operator->(){ return &(operator*()); }
                
        ndarray_iterator<T> &operator++(){
            ++pos_;
            return *this;
        }
        ndarray_iterator<T> operator++(int){
            T tmp(*this);
            ++(*this);
            return tmp;
        }
        ndarray_iterator<T> operator+(difference_type n){
            T tmp(*this);
            tmp.pos_ += n;
            return tmp;
        }
        ndarray_iterator<T> &operator+=(difference_type n){
            pos_ += n;
            return *this;
        }

        bool operator==(ndarray_iterator<T> b){
            return this->pos_ == b.pos_ && this->buf_ == b.buf_;
        }    
        bool operator!=(ndarray_iterator<T> b){
            return this->pos_ != b.pos_ || this->buf_ != b.buf_;
        }

    private:
        cbuf_type *buf_;
        size_t pos_;
};


#endif
