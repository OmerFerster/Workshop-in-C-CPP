#include "Matrix.h"

Matrix::Matrix (int rows, int cols) : _rows (rows), _cols (cols)
{
  if (this->_rows <= ZERO || this->_cols <= ZERO)
    {
      throw std::length_error (LENGTH_ERR);
    }
  this->_matrix = new float *[_rows];
  for (int i = 0; i < _rows; i++)
    {
      this->_matrix[i] = new float[_cols];
      for (int j = 0; j < _cols; j++)
        {
          this->_matrix[i][j] = 0;
        }
    }
}

Matrix::Matrix () : Matrix (ONE, ONE)
{}

Matrix::Matrix (const Matrix &m) : Matrix (m._rows, m._cols)
{
  for (int i = 0; i < this->_rows; i++)
    {
      for (int j = 0; j < this->_cols; j++)
        {
          this->_matrix[i][j] = (m._matrix)[i][j];
        }
    }
}

void Matrix::bye_matrix ()
{
  for (int i = 0; i < this->_rows; i++)
    {
      if (this->_matrix[i] != nullptr)
        {
          delete[] (this->_matrix)[i];
          (this->_matrix)[i] = nullptr;
        }
    }
  delete[] this->_matrix;
  this->_matrix = nullptr;
}

Matrix::~Matrix ()
{
  this->bye_matrix ();  // Apply Destructor helper function bye_matrix()
}

int Matrix::get_rows () const
{
  return this->_rows;
}

int Matrix::get_cols () const
{
  return this->_cols;
}

Matrix &Matrix::transpose ()
{
  Matrix new_matrix (this->_cols, this->_rows);
  for (int i = 0; i < this->_rows; i++)
    {
      for (int j = 0; j < this->_cols; j++)
        {
          float temp_value = this->_matrix[i][j];
          new_matrix._matrix[j][i] = temp_value;
        }
    }
  this->bye_matrix ();
  *this = new_matrix;
  return *this;
}

Matrix &Matrix::vectorize ()
{
  Matrix new_matrix (this->_rows * this->_cols, ONE);
  for (int i = 0; i < this->_rows * this->_cols; i++)
    {
      new_matrix._matrix[i][ZERO] = (*this)[i];
    }
  *this = new_matrix;
  return *this;
}

void Matrix::plain_print () const
{
  for (int i = 0; i < this->_rows; i++)
    {
      for (int j = 0; j < this->_cols; j++)
        {
          std::cout << this->_matrix[i][j] << SPACE;
        }
      std::cout << std::endl;
    }
}

Matrix Matrix::dot (const Matrix &m) const
{
  if ((this->_rows != m._rows) || (this->_cols != m._cols))
    {
      throw std::length_error (LENGTH_ERR);
    }
  Matrix result (this->_rows, this->_cols);
  for (int i = 0; i < this->_rows; i++)
    {
      for (int j = 0; j < this->_cols; j++)
        {
          result._matrix[i][j] = this->_matrix[i][j] * m._matrix[i][j];
        }
    }
  return result;
}

float Matrix::norm () const
{
  float sum = 0;
  for (int i = 0; i < this->_rows; i++)
    {
      for (int j = 0; j < this->_cols; j++)
        {
          sum += powf ((this->_matrix[i][j]), TWO);
        }
    }
  return std::sqrt (sum);
}

Matrix Matrix::operator+ (const Matrix &rhs) const
{
  if ((this->_rows != rhs._rows) || (this->_cols != rhs._cols))
    {
      throw std::length_error (LENGTH_ERR);
    }
  Matrix result (this->_rows, this->_cols);
  for (int i = 0; i < this->_rows; i++)
    {
      for (int j = 0; j < this->_cols; j++)
        {
          result._matrix[i][j] += (this->_matrix[i][j] + rhs._matrix[i][j]);
        }
    }
  return result;
}

Matrix &Matrix::operator= (const Matrix &rhs)
{
  if (this == &rhs)
    {
      return *this;
    }
  if (this->_matrix != nullptr)
    {
      this->bye_matrix ();
    }
  this->_rows = rhs._rows;
  this->_cols = rhs._cols;
  this->_matrix = new float *[this->_rows];
  for (int i = 0; i < this->_rows; i++)
    {
      this->_matrix[i] = new float[this->_cols];
      for (int j = 0; j < this->_cols; j++)
        {
          this->_matrix[i][j] = rhs._matrix[i][j];
        }
    }
  return *this;
}

Matrix Matrix::operator* (const Matrix &rhs) const
{
  if (this->_cols != rhs._rows)
    {
      throw std::length_error (LENGTH_ERR);
    }
  Matrix res (this->_rows, rhs._cols);
  for (int i = 0; i < res._rows; i++)
    {
      for (int j = 0; j < res._cols; j++)
        {
          float final = 0;
          for (int k = 0; k < rhs._rows; k++)
            {
              final += (this->_matrix[i][k] * rhs._matrix[k][j]);
            }
          res._matrix[i][j] += final;
        }
    }
  return res;
}

Matrix Matrix::operator* (const float &rhs) const
{
  Matrix res (this->_rows, this->_cols);
  for (int i = 0; i < this->_rows; i++)
    {
      for (int j = 0; j < this->_cols; j++)
        {
          res._matrix[i][j] += (rhs * this->_matrix[i][j]);
        }
    }
  return res;
}

Matrix operator* (const float &lhs, const Matrix &rhs)
{
  Matrix res (rhs.get_rows (), rhs.get_cols ());
  for (int i = 0; i < rhs.get_rows (); i++)
    {
      for (int j = 0; j < rhs.get_cols (); j++)
        {
          res._matrix[i][j] += (lhs * rhs._matrix[i][j]);
        }
    }
  return res;
}

Matrix &Matrix::operator+= (const Matrix &rhs)
{
  if ((this->_cols != rhs._cols) || (this->_rows != rhs._rows))
    {
      throw std::length_error (LENGTH_ERR);
    }
  for (int i = 0; i < this->_rows; i++)
    {
      for (int j = 0; j < this->_cols; j++)
        {
          this->_matrix[i][j] += rhs._matrix[i][j];
        }
    }
  return *this;
}

float Matrix::operator() (int i, int j) const
{
  if ((this->_rows <= i) || (this->_cols <= j) || (i < 0) || (j < 0))
    {
      throw std::out_of_range (OUT_OF_RANGE);
    }
  return this->_matrix[i][j];
}

float &Matrix::operator() (int i, int j)
{
  {
    if ((this->_rows <= i) || (this->_cols <= j) || (i < 0) || (j < 0))
      {
        throw std::out_of_range (OUT_OF_RANGE);
      }
    return this->_matrix[i][j];
  }
}

float Matrix::operator[] (int coord) const
{
  coord += 1;
  int row_counter = 0;
  while (coord > this->_cols)
    {
      row_counter++;
      coord -= this->_cols;
    }
  return (*this) (row_counter, coord - 1);
}

float &Matrix::operator[] (int coord)
{
  coord += 1;
  int row_counter = 0;
  while (coord > this->_cols)
    {
      row_counter++;
      coord -= this->_cols;
    }
  return (*this) (row_counter, coord - 1);
}

std::ostream &operator<< (std::ostream &os, Matrix &rhs)
{
  for (int i = 0; i < rhs.get_rows (); i++)
    {
      for (int j = 0; j < rhs.get_cols (); j++)
        {
          if (rhs (i, j) > ZERO_POINT_ONE)
            {
              os << DOUBLE_STARS;
            }
          else
            {
              os << DOUBLE_SPACES;
            }
        }
      os << std::endl;
    }
  return os;
}

std::istream &operator>> (std::istream &is, Matrix &rhs)
{
  if (is)
    {
      is.seekg (0, std::istream::end);
      int length = (int) is.tellg ();
      is.seekg (0, std::istream::beg);
      if ((unsigned int) length / (unsigned int) sizeof (float)
          < (unsigned int) (rhs._cols * rhs._rows))
        {
          throw std::length_error (LENGTH_ERR);
        }
      int i = 0;
      while ((i < rhs._rows * rhs._cols) && is.read
          (reinterpret_cast<char *>(&rhs[i]), sizeof (float)))
        {
          i++;
        }
    }
  else if (!is)
    {
      throw std::runtime_error (RUN_TIME_ERROR);
    }
  return is;
}
