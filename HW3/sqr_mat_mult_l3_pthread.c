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
#include <pthread.h>

#define NUM_THREADS 2

struct args {
	int n;
	int s;
	int* threadid;
};
/*declare global variables*/
double* A=NULL;
double* B=NULL;
double* C = NULL;
/*declare functions*/
void Get_dims(int* n_p);
void *Mat_Mat_mult(void *arguments);
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

/*-------------------------------------------------------------------*/
int main(void) {
   double* A = NULL;
   double* B = NULL;
   double c = 0.0;
   int n, i, j;
   clock_t start, end;
   double cpu_time_used;
   pthread_t threads[NUM_THREADS];

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
struct args *mult_args;

  mult_args->n = n;
  mult_args->s = 250;
  mult_args->threadid = 0;
start = clock();
int t,rc;
for(t=0;t<NUM_THREADS;t++){
	mult_args->threadid =&t;
  rc = pthread_create(&threads[t], NULL, Mat_Mat_mult, &mult_args);
  if (rc){
    printf("ERROR; return code from pthread_create() is %d\n", rc);
    exit(-1);
    }
 
   
}
/*for(t=0;t<NUM_THREADS;t++){
	(void)pthread_join(threads[t], NULL);

}*/
end = clock();
   //Mat_Mat_mult(A, B, C, n, s);

   cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
   printf("The Loop Runtime is %f ", cpu_time_used);

   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
         c += C[i*n+j];
      }
   }
   c = c / (n*n);
   printf("The average of the matrix is %f ", c);
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

void *Mat_Mat_mult(void *arguments) {

   int i, j, k, it, jt, kt,my_beg, my_fin, my_n, s, n, h,my_tid ;// 
   long tid;
   double* my_A = NULL;
   double* my_B = NULL;
   double* my_C = NULL;

   struct args *my_args;
   my_args = (struct args *) arguments; 
   tid = (long) my_args->threadid;
   my_tid=(int) tid;
   n = (int) my_args -> n;
   my_n = n/NUM_THREADS;
   
   s = (int) my_args -> s;


   my_beg=tid*(n/NUM_THREADS);
   my_fin= (tid+1)*(n/NUM_THREADS);
   my_A = malloc(n*n/NUM_THREADS*sizeof(double));
   my_B = malloc(n*n/NUM_THREADS*sizeof(double));
   my_C = malloc(n*n/NUM_THREADS*sizeof(double));
   h= 0;
   if(h<my_n){
      for (i = my_beg; i < my_fin; i++) {
          for (j = 0; j < my_n; j++) {
	         my_A[n*h+j] = A[i*n+j];
	         my_B[n*h+j] = B[i*n+j];
	         my_C[n*h+j] = C[i*n+j];
          }
         h++;
      }
   }
  for (it = 0; it < my_n; it+=s) {
	  for (kt = 0; kt < my_n; kt+=s) {
		  for (jt = 0; jt < my_n; jt+=s) {
		     for (i = it; i < MIN(it+374,n/2); i++) {
			for (k = kt; k < MIN(kt+s-1,n); k++) {
			   for (j = jt; j < MIN(jt+s-1,n); j++) {
			      C[i*n+j] += my_A[i*n+k]*my_B[j*n+k];
			   }
			}
		     }
		  }
	  }
  }

pthread_mutex_lock(&mutex1);
h=0;
for(i=my_beg; i<my_fin; i++){
   for (j=0; j<n; j++){
        C[i*n+j] += my_C[n*h+j];
   }
   h++;
}
printf("Thread %i has finished its section.", my_tid);
pthread_mutex_unlock(&mutex1);
return NULL;

   
   
} /* Mat_Mat_mult */




