/*mymatrix.h*/

// 
// Niko Castellana
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #02
//

//
// mymatrix
//
// The mymatrix class provides a matrix (2D array) abstraction.
// The size can grow dynamically in both directions (rows and 
// cols).  Also, rows can be "jagged" --- i.e. rows can have 
// different column sizes, and thus the matrix is not necessarily 
// rectangular.  All elements are initialized to the default value
// for the given type T.  Example:
//
//   mymatrix<int>  M;  // 4x4 matrix, initialized to 0
//   
//   M(0, 0) = 123;
//   M(1, 1) = 456;
//   M(2, 2) = 789;
//   M(3, 3) = -99;
//
//   M.growcols(1, 8);  // increase # of cols in row 1 to 8
//
//   for (int r = 0; r < M.numrows(); ++r)
//   {
//      for (int c = 0; c < M.numcols(r); ++c)
//         cout << M(r, c) << " ";
//      cout << endl;
//   }
//
// Output:
//   123 0 0 0
//   0 456 0 0 0 0 0 0
//   0 0 789 0
//   0 0 0 -99
//

#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>

using namespace std;

template<typename T>
class mymatrix
{
private:
  struct ROW
  {
    T*  Cols;     // dynamic array of column elements
    int NumCols;  // total # of columns (0..NumCols-1)
  };

  ROW* Rows;     // dynamic array of ROWs
  int  NumRows;  // total # of rows (0..NumRows-1)

public:
  //
  // default constructor:
  //
  // Called automatically by C++ to construct a 4x4 matrix.  All 
  // elements are initialized to the default value of T.
  //
  mymatrix()
  {
    Rows = new ROW[4];  // an array with 4 ROW structs:
    NumRows = 4;

    // initialize each row to have 4 columns:
    for (int r = 0; r < NumRows; ++r)
    {
      Rows[r].Cols = new T[4];  // an array with 4 elements of type T:
      Rows[r].NumCols = 4;

      // initialize the elements to their default value:
      for (int c = 0; c < Rows[r].NumCols; ++c)
      {
        Rows[r].Cols[c] = T{};  // default value for type T:
      }
    }
  }

  
//   parameterized constructor:
  
//   Called automatically by C++ to construct a matrix with R rows, 
//   where each row has C columns. All elements are initialized to 
//   the default value of T.
  
  mymatrix(int R, int C)
  {
    if (R < 1)
      throw invalid_argument("mymatrix::constructor: # of rows");
    if (C < 1)
      throw invalid_argument("mymatrix::constructor: # of cols");
    
    NumRows = R;
    Rows = new ROW[R];  // array with R # of Rows
    
    // initialize each row to have C # of columns
    for (int r = 0; r < R; ++r)
    {
      Rows[r].Cols = new T[C];  // an array with elements of type T:
      Rows[r].NumCols = C;

      // initialize the elements to their default value:
      for (int c = 0; c < Rows[r].NumCols; ++c)
      {
        Rows[r].Cols[c] = T{};  // default value for type T:
      }
      
    }
    
  }


  //
  // copy constructor:
  //
  // Called automatically by C++ to construct a matrix that contains a 
  // copy of an existing matrix.  Example: this occurs when passing 
  // mymatrix as a parameter by value
  //
  //   void somefunction(mymatrix<int> M2)  <--- M2 is a copy:
  //   { ... }
  //
  mymatrix(const mymatrix<T>& other)
  {
    this -> Rows = new ROW[other.NumRows];  // Copy array rows to other matrix
    this -> NumRows = other.NumRows;        // Copy # of rows to other matrix
    
    // Loop through the rows and columns of the other matrix
    for(int r = 0; r < other.NumRows; ++r)
    {
        this -> Rows[r].Cols = new T[other.Rows[r].NumCols]; // Copy array columns to other matrix
        this -> Rows[r].NumCols = other.Rows[r].NumCols;     // Copy # of columns to other matrix
                                               
        for(int c = 0; c < other.Rows[r].NumCols; ++c)
        {
          this -> Rows[r].Cols[c] = other.Rows[r].Cols[c];   // Copy elements to other matrix
        }
    }
  }


  //
  // numrows
  //
  // Returns the # of rows in the matrix.  The indices for these rows
  // are 0..numrows-1.
  //
  int numrows() const
  {
      return NumRows;  // Return the # of rows
  }
  

  //
  // numcols
  //
  // Returns the # of columns in row r.  The indices for these columns
  // are 0..numcols-1.  Note that the # of columns can be different 
  // row-by-row since "jagged" rows are supported --- matrices are not
  // necessarily rectangular.
  //
  int numcols(int r) const
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::numcols: row");
    
    return Rows[r].NumCols;  // Return the # of columns in the row
  }


  //
  // growcols
  //
  // Grows the # of columns in row r to at least C.  If row r contains 
  // fewer than C columns, then columns are added; the existing elements
  // are retained and new locations are initialized to the default value 
  // for T.  If row r has C or more columns, then all existing columns
  // are retained -- we never reduce the # of columns.
  //
  // Jagged rows are supported, i.e. different rows may have different
  // column capacities -- matrices are not necessarily rectangular.
  //
  void growcols(int r, int C)
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::growcols: row");
    if (C < 1)
      throw invalid_argument("mymatrix::growcols: columns");
      
    if (C <= Rows[r].NumCols)   // Prevents shrinking columns
        return;
    
    T* newCol = new T[C];       // Grow row to have C columns
    
    for(int i = 0; i < Rows[r].NumCols; ++i) // Fill in new row with the old row values
    {
        newCol[i] = Rows[r].Cols[i];
    }
    
    for(int j = Rows[r].NumCols; j < C; j++)  // Initialize the new row to default value
    {
        newCol[j] = T{};
    }
    
    delete[] Rows[r].Cols;     // Delete old row
    Rows[r].Cols = newCol;     // Update new row
    Rows[r].NumCols = C;       // Update number of columns
  }


  //
  // grow
  //
  // Grows the size of the matrix so that it contains at least R rows,
  // and every row contains at least C columns.
  // 
  // If the matrix contains fewer than R rows, then rows are added
  // to the matrix; each new row will have C columns initialized to 
  // the default value of T.  If R <= numrows(), then all existing
  // rows are retained -- we never reduce the # of rows.
  //
  // If any row contains fewer than C columns, then columns are added
  // to increase the # of columns to C; existing values are retained
  // and additional columns are initialized to the default value of T.
  // If C <= numcols(r) for any row r, then all existing columns are
  // retained -- we never reduce the # of columns.
  // 
  void grow(int R, int C)
  {
    if (R < 1)
      throw invalid_argument("mymatrix::grow: # of rows");
    if (C < 1)
      throw invalid_argument("mymatrix::grow: # of cols");
     
    if (R < NumRows)  // Set R = NumRows to avoid shrinking matrix
    {
        R = NumRows;
    }
        
    mymatrix<T> temp(R,C);  // Declare temporary matrix
    
    // Loop through the matrix
    for (int i = 0; i < NumRows; ++i)
    {
        if(Rows[i].NumCols > C)  // Grow the columns in temp
            temp.growcols(i,Rows[i].NumCols);
            
        for(int j = 0; j < Rows[i].NumCols; j++)  // Copy elements from matrix to temp
            temp.Rows[i].Cols[j] = Rows[i].Cols[j];
    }
    
    *this = temp;  // Set primary matrix equal to temp

  }
  //
  // size
  //
  // Returns the total # of elements in the matrix.
  //
  int size() const
  {
     int size = 0; // Size of matrix
      
     for(int i = 0; i < NumRows; ++i) // Loop through matrix
     {
         for(int j = 0; j < Rows[i].NumCols; ++j)
            size++;  // Increment size
     }
         
    return size;
  }


  //
  // at
  //
  // Returns a reference to the element at location (r, c); this
  // allows you to access the element or change it:
  //
  //    M.at(r, c) = ...
  //    cout << M.at(r, c) << endl;
  //
  T& at(int r, int c)
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::at: row");
    if (c < 0 || c >= Rows[r].NumCols)
      throw invalid_argument("mymatrix::at: col");

    return Rows[r].Cols[c];
  }


  //
  // ()
  //
  // Returns a reference to the element at location (r, c); this
  // allows you to access the element or change it:
  //
  //    M(r, c) = ...
  //    cout << M(r, c) << endl;
  //
  T& operator()(int r, int c)
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::(): row");
    if (c < 0 || c >= Rows[r].NumCols)
      throw invalid_argument("mymatrix::(): col");

    return Rows[r].Cols[c];
  }

  //
  // scalar multiplication
  //
  // Multiplies every element of this matrix by the given scalar value,
  // producing a new matrix that is returned.  "This" matrix is not
  // changed.
  //
  // Example:  M2 = M1 * 2;
  //
  mymatrix<T> operator*(T scalar)
  {
    mymatrix<T> result(*this);  // Result points to the matrix
    
    // Loop through the matrix
    for(int i = 0; i < NumRows; ++i)
    {
        for(int j = 0; j < Rows[i].NumCols; ++j)
            result(i,j) = Rows[i].Cols[j] * scalar;  // Multiply values in matrix
    }
    
    return result;
  }


  //
  // matrix multiplication
  //
  // Performs matrix multiplication M1 * M2, where M1 is "this" matrix and
  // M2 is the "other" matrix.  This produces a new matrix, which is returned.
  // "This" matrix is not changed, and neither is the "other" matrix.
  //
  // Example:  M3 = M1 * M2;
  //
  // NOTE: M1 and M2 must be rectangular, if not an exception is thrown.  In
  // addition, the sizes of M1 and M2 must be compatible in the following sense:
  // M1 must be of size RxN and M2 must be of size NxC.  In this case, matrix
  // multiplication can be performed, and the resulting matrix is of size RxC.
  //
  mymatrix<T> operator*(const mymatrix<T>& other)
  {
    // Loop through the # rows to check if the matrix is rectangular
    for (int i = 0; i < NumRows; ++i)
    {
        if(Rows[i].NumCols != Rows[0].NumCols)  // Check if # columns are equal
            throw runtime_error("mymatrix::*: this not rectangular");
    }
    
    // Loop through the # rows to check if the other matrix is rectangular
    for (int j = 0; j < other.NumRows; ++j)
    {
        if(other.Rows[j].NumCols != other.Rows[0].NumCols)  // Check if # columns are equal
            throw runtime_error("mymatrix::*: other not rectangular");
    }
    
    // Check to make sure matrix multiplication can be performed
    if (Rows[0].NumCols != other.NumRows)
      throw runtime_error("mymatrix::*: size mismatch");

    // Declare result for matrix multiplication
    mymatrix<T> result(NumRows, other.Rows[0].NumCols);

    // Loop through the # rows of original matrix
    for(int a = 0; a < NumRows ; ++a)
    {
        for(int b = 0; b < other.Rows[0].NumCols ; ++b) // Loop through # columns of other matrix
        {
            result(a,b) = 0;  // Initialize result
            for(int c = 0; c < other.NumRows; ++c)  // Loop through # rows of other matrix
            {
               result(a,b) += Rows[a].Cols[c] * other.Rows[c].Cols[b];  // Use matrix multiplication
            }
        }
            
    }
        
    return result;
  }

//   _output
//   
//   Outputs the contents of the matrix; for debugging purposes.
  
  void _output()
  {
    for (int r = 0; r < this->NumRows; ++r)
    {
      for (int c = 0; c < this->Rows[r].NumCols; ++c)
      {
        cout << this->Rows[r].Cols[c] << " ";
      }
      cout << endl;
    }
  }

};
