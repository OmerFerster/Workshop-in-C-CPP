#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <cmath>
#include <fstream>

#define LENGTH_ERR "The matrix size given is invalid"
#define OUT_OF_RANGE "The range given is invalid"
#define RUN_TIME_ERROR "The input is invalid"
#define TWO 2
#define ONE 1
#define ZERO 0
#define ZERO_POINT_ONE 0.1
#define DOUBLE_STARS "**"
#define DOUBLE_SPACES "  "
#define SPACE " "

typedef struct matrix_dims {
    int rows, cols;
} matrix_dims;

class Matrix {

 private:
  int _rows;
  int _cols;
  float **_matrix;
  void bye_matrix ();  // Destruct A matrix from the program

 public:
  /**
   * The Constructor function, apply fields
   * @param rows Number of rows
   * @param cols Number of cols
   */
  Matrix (int rows, int cols);
  Matrix ();
  /**
   * The Copy Constructor, copied m fields to a new matrix.
   * @param m The matrix to copy
   */
  Matrix (const Matrix &m);
  ~Matrix ();
  int get_rows () const;
  int get_cols () const;
  /**
   * The function Transpose the matrix.
   * @return this transposed matrix
   */
  Matrix &transpose ();
  /**
   * The function change A n*n matrix into (n*n)*1 column vector
   * @return The new vectorized matrix
   */
  Matrix &vectorize ();
  /**
   * The function prints the matrix info
   */
  void plain_print () const;
  /**
   * The function compute a dot multiplication of both this & given matrices
   * @param m The given matrix to compute dot with this->_matrix
   * @return The new matrix output from the dot multiplication
   */
  Matrix dot (const Matrix &m) const;
  /**
   * The function compute the matrix norm
   * @return The norm value of matrix
   */
  float norm () const;

  // -- The Class Operators -- //
  Matrix operator+ (const Matrix &rhs) const;
  Matrix &operator= (const Matrix &rhs);
  Matrix operator* (const Matrix &rhs) const;
  Matrix operator* (const float &rhs) const;
  friend Matrix operator* (const float &lhs, const Matrix &rhs);
  Matrix &operator+= (const Matrix &rhs);
  float &operator() (int i, int j);
  float operator() (int i, int j) const;
  float &operator[] (int coord);
  float operator[] (int coord) const;
  friend std::ostream &operator<< (std::ostream &os, Matrix &rhs);
  friend std::istream &operator>> (std::istream &is, Matrix &rhs);
};

#endif //MATRIX_H
