/* File:     mat_vect_mult.c
 *
 * Purpose:  Implement serial matrix-vector multiplication using
 *           one-dimensional arrays to store the vectors and the
 *           matrix.
 *
 * Compile:  gcc -g -Wall -o mat_vect_mult mat_vect_mult.c
 * Run:      ./mat_vect_mult
 *
 * Input:    Dimensions of the matrix (m = number of rows, n
 *              = number of columns)
 *           n-dimensional vector x
 * Output:   Product vector y = Ax
 *
 * Errors:   if the number of user-input rows or column isn't
 *           positive, the program prints a message and quits.
 * Note:     Define DEBUG for verbose output
 *
 */
#include <stdio.h>
#include <stdlib.h>

void Get_dims(int* m_p, int* n_p);
void Read_matrix(char prompt[], double A[], int m, int n);
void Print_matrix(char title[], double A[], int m, int n);
void Mat_mat_mult(double A[], double B[], double C[], int m, int n, int x, int y);

/*-------------------------------------------------------------------*/
int main(void) {
   double* A = NULL;
   double* B = NULL;
   double* C = NULL;
   int m, n, x, y;

   Get_dims(&m, &n);
   A = malloc(m*n*sizeof(double));
   B = malloc(x*y*sizeof(double));
   C = malloc(m*y*sizeof(double));
   if (A == NULL || B == NULL || C == NULL) {
      fprintf(stderr, "Can't allocate storage\n");
      exit(-1);
   }
   Read_matrix("A", A, m, n);
#  ifdef DEBUG
   Print_matrix("A", A, m, n);
#  endif
   Get_dims(&x, &y);
   if (x!=n) {
      fprintf(stderr, "The dementions of x and n must match to multiply the Matices \n");
      exit(-1);
   }
   Read_matrix("B", B, x, y);
#  ifdef DEBUG
   Print_matrix("B", B, x, y);
#  endif


   Mat_mat_mult(A, B, C, m, n, x, y);

   Print_matrix("C", C, m, y);

   free(A);
   free(B);
   free(C);
   return 0;
}  /* main */


/*-------------------------------------------------------------------
 * Function:   Get_dims
 * Purpose:    Read the dimensions of the matrix from stdin
 * Out args:   m_p:  number of rows
 *             n_p:  number of cols
 *
 * Errors:     If one of the dimensions isn't positive, the program
 *             prints an error and quits
 */
void Get_dims(
              int*  m_p  /* out */, 
              int*  n_p  /* out */) {
   printf("Enter the number of rows\n");
   scanf("%d", m_p);
   printf("Enter the number of columns\n");
   scanf("%d", n_p);

   if (*m_p <= 0 || *n_p <= 0) {
      fprintf(stderr, "m and n must be positive\n");
      exit(-1);
   }
}  /* Get_dims */

/*-------------------------------------------------------------------
 * Function:   Read_matrix
 * Purpose:    Read the contents of the matrix from stdin
 * In args:    prompt:  description of matrix
 *             m:       number of rows
 *             n:       number of cols
 * Out arg:    A:       the matrix
 */
void Read_matrix(
                 char    prompt[]  /* in  */, 
                 double  A[]       /* out */, 
                 int     m         /* in  */, 
                 int     n         /* in  */) {
   int i, j;

   printf("Enter the matrix %s\n", prompt);
   for (i = 0; i < m; i++)
      for (j = 0; j < n; j++)
         scanf("%lf", &A[i*n+j]);
}  /* Read_matrix */



/*-------------------------------------------------------------------
 * Function:   Print_matrix
 * Purpose:    Print a matrix to stdout
 * In args:    title:  title for output
 *             A:      the matrix
 *             m:      number of rows
 *             n:      number of cols
 */
void Print_matrix(
                  char    title[]  /* in */,
                  double  A[]      /* in */, 
                  int     m        /* in */, 
                  int     n        /* in */) {
   int i, j;

   printf("\nThe matrix %s\n", title);
   for (i = 0; i < m; i++) {
      for (j = 0; j < n; j++)
         printf("%f ", A[i*n+j]);
      printf("\n");
   }
}  /* Print_matrix */




/*-------------------------------------------------------------------
 * Function:   Mat_mat_mult
 * Purpose:    Multiply a matrix by a vector
 * In args:    A: the first matrix
 *             B: the matrix being multiplied by A
 *             m: the number of rows in A and C
 *             n: the number of columns in A and rows in B
 *	       x: the number of rows in B and columns in A
 *             y:the number of columns in B and columns in C
 * Out args:   C: the product vector AB
 */
void Mat_mat_mult(
                   double  A[]  /* in  */, 
                   double  B[]  /* in  */, 
                   double  C[]  /* out */,
                   int     m    /* in  */, 
                   int     n    /* in  */,
                   int     x    /* in  */, 
                   int     y    /* in  */) {
   int i, j, k;

   for (i = 0; i < m; i++) {
      C[i] = 0.0;
      for (j = 0; j < n; j++){
	  for (k = 0; k<y; k++)
            C[i] += A[i*n+k]*B[k*x + j];
    }
  }
}  /* Mat_vect_mult */
