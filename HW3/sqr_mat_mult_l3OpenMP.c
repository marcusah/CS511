/* File:     loop3.c
 *
 * Purpose:  Implement loop 3 from Homework 1 of CS 511
 *
 * Compile:  gcc -g -Wall -o loop3.c
 * Run:      ./loop3
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
#include <omp.h>

#define NUM_THREADS 4


/*declare global variables
double* A=NULL;
double* B=NULL;
double* C = NULL;*/
/*declare functions*/
void Get_dims(int* n_p);
void Mat_Mat_mult(double A[], double B[], double C[], int n, int s);

/*-------------------------------------------------------------------*/
int main(void) {
   double* A = NULL;
   double* B = NULL;
   double* C = NULL;
   double c = 0.0;
   int n, i, j, s;
   clock_t  start, end;
   time_t wall_start, wall_end;
   double cpu_time_used, wall_time_used;
  
   Get_dims(&n);
   A = malloc(n*n*sizeof(double));
   B = malloc(n*n*sizeof(double));
   C = malloc(n*n*sizeof(double));

   if (A == NULL || B == NULL || C == NULL) {
      fprintf(stderr, "Can't allocate storage\n");
      exit(-1);
   }

   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
        A[i*n+j] = 2.0;
      }
   }

   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
        B[i*n+j] = 3.0;
      }
   }

   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
        C[i*n+j] = 0.0;
      }
   }

  s = 250;
  wall_start = time(NULL);
	   Mat_Mat_mult(A, B, C, n, s);
 wall_end = time(NULL);
  
wall_time_used = wall_end - wall_start; 
printf("The Loop wall time is %f \n ", wall_time_used);
start = clock();
  

//#pragma omp parallel for default(shared) private(<loop iterators>) schedule(static, <block size>) num_threads(<number of threads supported by machine>)


   Mat_Mat_mult(A, B, C, n, s);

   
   end = clock();
   cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
   printf("The Loop Runtime is %f \n ", cpu_time_used);

   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
         c += C[i*n+j];
      }
   }
   c = c / (n*n);
   printf("The average of the matrix is %f \n ", c);
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
/*
   printf("Enter the cache size for double values\n");
   scanf("%d", s_p);

   if (*s_p <= 0) {
      fprintf(stderr, "s must be positive\n");
      exit(-1);
   }

*/
}  /* Get_dims */

/*-------------------------------------------------------------------
 * Function:   Mat_Mat_mult
 * Purpose:    Multiply a matrix by a matrix
 * In args:    A: the matrix
 *             B: the matrix being multiplied by A
 *             n: the dimension of matrices A and B
 *             s: 'estimated' cache size for double values
 * Out args:   C: the product matrix AB
 *  L1 Cache: 16K - Each double is 64 bits (8 bytes)
 *     Cache should be able to hold ~250values
 */
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void Mat_Mat_mult(
                   double  A[]  /* in  */,
                   double  B[]  /* in  */,
                   double  C[]  /* out */,
                   int     n    /* in  */,
                   int     s    /* in  */) {
   int i, j, k, it, jt, kt;
#pragma omp parallel for default(shared) private(i,j,k,it,jt,kt) schedule(static, n/NUM_THREADS) num_threads(NUM_THREADS)  
  for (it = 0; it < n; it+=s) {
	  for (kt = 0; kt < n; kt+=s) {
		  for (jt = 0; jt < n; jt+=s) {
		     for (i = it; i < MIN(it+s-1,n); i++) {
			for (k = kt; k < MIN(kt+s-1,n); k++) {
			   for (j = jt; j < MIN(jt+s-1,n); j++) {
			      C[i*n+j] += A[i*n+k]*B[j*n+k];
			   }
			}
		     }
		  }
	  }
  }
   
   
} /* Mat_Mat_mult */





