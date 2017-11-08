
/* File:     loop1.c
 *
 * Purpose:  Implement loop 1 from Homework 1 of CS 511
 *
 * Compile:  gcc -g -Wall -o HW1 loop1.c
 * Run:      ./loop1
 *
 * Input:    Dimensions of the matrix a (n = number of rows, n
 *              = number of columns)
 *           n x n dimensional matrix b
 * Output:   Product matrix c = a*b
 *
 * Errors:   if the number of user-input rows or column isn't
 *           positive, the program prints a message and quits.
 * Note:     Define DEBUG for verbose output
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void Get_dims(int* n_p);
void Mat_Mat_mult(double A[], double B[], double C[], int n);
void Print_matrix(char title[], double A[], int m, int n);

/*-------------------------------------------------------------------*/
int main(void) {
   double* A = NULL;
   double* B = NULL;
   double* C = NULL;
   double c =0.0;
   int n, i, j;
   clock_t start, end;
   double cpu_time_used;

   Get_dims(&n);
   A = malloc(n*n*sizeof(double));
   B = malloc(n*n*sizeof(double));
   C = malloc(n*n*sizeof(double));
   if (A == NULL || B == NULL || C == NULL) {
      fprintf(stderr, "Can't allocate storage\n");
      exit(-1);
   }
/*Initialize A,B,C*/
   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
        A[i*n+j] = 2.0;
      }
   }
//   Print_matrix("A",A,n,n);   
   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
        B[i*n+j] = 3.0;
      }
   }
//   Print_matrix("B",B,n,n);

   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
        C[i*n+j] = 0.0;
      }
}
   start = clock();

   Mat_Mat_mult(A, B, C, n);

   end = clock();

 //  Print_matrix("C",C,n,n);

   cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

   printf("The Loop Runtime (sec) is: %f", cpu_time_used);
   printf("\n");

   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
         c += C[i*n+j];
      }
   }

   c = c / (n*n);
   printf("The average of the matrix is: %f", c );
   printf("\n");
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
void Get_dims(int*  n_p  /* out */) {
   printf("Enter the dimension of the matrix\n");
   scanf("%d", n_p);

   if (*n_p <= 0) {
      fprintf(stderr, "n must be positive\n");
      exit(-1);
   }
}  /* Get_dims */
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
 * Function:   Mat_Mat_mult
 * Purpose:    Multiply a matrix by a matrix
 * In args:    A: the matrix
 *             B: the matrix being multiplied by A
 *             n: the dimension of matrices A and B
 * Out args:   C: the product matrix AB
 */
void Mat_Mat_mult(
                   double  A[]  /* in  */,
                   double  B[]  /* in  */,
                   double  C[]  /* out */,
                   int     n    /* in  */) {
   int i, j, k;

   for (i = 0; i < n; i++) {
      for (k = 0; k < n; k++){
         for (j = 0; j < n; j++) {
 
            C[i*n+j] += A[i*n+k]*B[j*n+k];
         }
      }
   }
}  /* Mat_Mat_mult */




