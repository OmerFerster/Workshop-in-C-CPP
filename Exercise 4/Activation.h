#ifndef ACTIVATION_H
#define ACTIVATION_H
#include "Matrix.h"

#define MINUS_ONE -1


namespace activation
{
    typedef Matrix (*active_func_type) (Matrix &);
    Matrix relu (Matrix &vec);
    Matrix softmax (Matrix &vec);
}

#endif //ACTIVATION_H