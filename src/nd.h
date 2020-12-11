#ifndef H_ND
#define H_ND

#include "../src/ndarray.h"


namespace nd {
    /* Namespace for ndarray related functions. */

    // Creation

    // Functional
    template <typename T>
    extern ndarray<T> append(ndarray<T>&, ndarray<T>&);

    template <typename T>
    extern ndarray<T> copy(ndarray<T>&);

    template <typename T>
    extern ndarray<T> resize(ndarray<T>&, const int&, const T&);

    template <typename T>
    extern ndarray<T> squeeze(ndarray<T>&);

    // Logical
    template <typename T>
    extern ndarray<T> unique(ndarray<T>);
}


# endif
