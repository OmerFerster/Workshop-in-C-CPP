#include "Activation.h"

Matrix activation::relu (Matrix &vec)
{
  if (vec.get_cols () != ONE)
    {
      vec.vectorize();
    }
  Matrix output_vec (vec.get_rows (), ONE);
  for (int i = 0; i < vec.get_rows (); i++)
    {
      float new_val = (vec (i, ZERO) >= ZERO) ? vec (i, ZERO) : ZERO;
      output_vec (i, ZERO) += new_val;
    }
  return output_vec;
}

Matrix activation::softmax (Matrix &vec)
{
  if (vec.get_cols () != ONE)
    {
      vec.vectorize();
    }
  float sum = 0;
  for (int i = 0; i < vec.get_rows (); i++)
    {
      sum += std::exp (vec (i, ZERO));
    }
  if (sum == ZERO)
    {
      throw std::runtime_error (RUN_TIME_ERROR);
    }
  sum = powf (sum, MINUS_ONE);
  Matrix output_vec (vec.get_rows (), ONE);
  for (int j = 0; j < vec.get_rows (); j++)
    {
      output_vec (j, ZERO) += (sum * (std::exp (vec (j, ZERO))));
    }
  return output_vec;
}
