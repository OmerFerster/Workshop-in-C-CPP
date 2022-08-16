#include "Dense.h"

Dense::Dense (const Matrix &weights, const Matrix &bias,
              active_func_type ActivationFunction)
    : _weights (weights), _bias (bias), _active_func (ActivationFunction)
{}

Matrix &Dense::get_weights ()
{
  return this->_weights;
}

Matrix &Dense::get_bias ()
{
  return this->_bias;
}

active_func_type Dense::get_activation ()
{
  return this->_active_func;
}

Matrix Dense::operator() (const Matrix &input_matrix) const
{
  Matrix new_mat = (this->_weights * input_matrix) + this->_bias;
  return this->_active_func (new_mat);
}
