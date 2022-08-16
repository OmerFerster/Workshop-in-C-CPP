#include "MlpNetwork.h"

MlpNetwork::MlpNetwork (Matrix *weights, Matrix *biases)
    : _weights (weights), _biases (biases)
{}

digit MlpNetwork::operator() (Matrix &input_matrix) const
{
  input_matrix.vectorize ();
  Dense l1 (this->_weights[ZERO], this->_biases[ZERO], activation::relu);
  Dense l2 (this->_weights[ONE], this->_biases[ONE], activation::relu);
  Dense l3 (this->_weights[TWO], this->_biases[TWO], activation::relu);
  Dense l4 (this->_weights[THREE], this->_biases[THREE], activation::softmax);
  Matrix layer_output = l4 (l3 (l2 (l1 (input_matrix))));
  float max = 0;
  unsigned int last_number = 0;
  for (int i = 0; i < layer_output.get_rows (); i++)
    {
      if (max < layer_output (i, ZERO))
        {
          max = layer_output (i, ZERO);
          last_number = i;
        }
    }
  return digit{last_number, max};
}