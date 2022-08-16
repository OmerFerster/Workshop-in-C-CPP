#ifndef DENSE_H
#define DENSE_H
#include "Activation.h"

using activation::active_func_type;

class Dense {

 private:
  Matrix _weights;
  Matrix _bias;
  active_func_type _active_func;

 public:
  Dense (const Matrix &weights, const Matrix &bias,
         active_func_type ActivationFunction);
  Matrix &get_weights ();
  Matrix &get_bias ();
  active_func_type get_activation ();
  Matrix operator() (const Matrix &input_matrix) const;
};

#endif //DENSE_H