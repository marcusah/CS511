/* trap.c -- Parallel Trapezoidal Rule, simple version
 *
 * Input: None.
 * Output:  Estimate of the integral from a to b of f(x)
 *    using the trapezoidal rule and n trapezoids.
 *
 * Algorithm:
 *    1.  Each process calculates "its" interval of
 *        integration.
 *    2.  Each process estimates the integral of f(x)
 *        over its interval using the trapezoidal rule.
 *    3a. Each process != 0 sends its integral to 0.
 *    3b. Process 0 sums the calculations received from
 *        the individual processes and prints the result.
 *
 * Notes:  
 *    1.  f(x), a, b, and n are all hardwired.
 *    2.  The number of processes (p) should evenly divide
 *        the number of trapezoids (n = 1024)
 *
 */
#include <stdio.h>
#include <time.h>
/* We'll be using MPI routines, definitions, etc. */
#include <mpi.h>


int main(int argc, char** argv) {
    int         my_rank;   /* My process rank           */
    int         p;         /* The number of processes   */
    float       a = 0.0;   /* Left endpoint             */
    float       b = 1.0;   /* Right endpoint            */
    int         n = 1024;  /* Number of trapezoids      */
    float       h;         /* Trapezoid base length     */
    float       local_a;   /* Left endpoint my process  */
    float       local_b;   /* Right endpoint my process */
    int         local_n;   /* Number of trapezoids for  */
                           /* my calculation            */
    float       integral;  /* Integral over my interval */
    float       total;     /* Total integral            */
    int         source;    /* Process sending integral  */
    int         dest = 0;  /* All messages go to 0      */
    int         tag = 0;
    MPI_Status  status;
    void Get_data1(float* a_ptr, float* b_ptr, int* n_ptr,
		 int my_rank, int p);
    void Get_data2(float* a_ptr, float* b_ptr, int* n_ptr, int my_rank);
    float Trap(float local_a, float local_b, int local_n,
              float h);    /* Calculate local integral  */
   clock_t  start, end;
   double cpu_time_used;
   start = clock();
    /* Let the system do what it needs to start up MPI */
    MPI_Init(&argc, &argv);

    /* Get my process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /* Find out how many processes are being used */
    MPI_Comm_size(MPI_COMM_WORLD, &p);
       printf("There are %i processes.\n ", p);
//    Get_data1(&a,&b,&n,my_rank,p);
    Get_data3(&a,&b,&n,my_rank);

    h = (b-a)/n;    /* h is the same for all processes */
    local_n = n/p;  /* So is the number of trapezoids */

    /* Length of each process' interval of
     * integration = local_n*h.  So my interval
     * starts at: */
    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;
    integral = Trap(local_a, local_b, local_n, h);

    /* Add up the integrals calculated by each process */
    if (my_rank == 0) {
        total = integral;
        for (source = 1; source < p; source++) {
            MPI_Recv(&integral, 1, MPI_FLOAT, source, tag,
                MPI_COMM_WORLD, &status);
            total = total + integral;
        }
    } else {  
        MPI_Send(&integral, 1, MPI_FLOAT, dest,
            tag, MPI_COMM_WORLD);
    }

    /* Print the result */
    if (my_rank == 0) {
        printf("With n = %d trapezoids, our estimate\n",
            n);
        printf("of the integral from %f to %f = %f\n",
            a, b, total);
    }

    /* Shut down MPI */
    MPI_Finalize();
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("The Loop Runtime is %f \n ", cpu_time_used);
return 0;
} /*  main  */


float Trap(
          float  local_a   /* in */,
          float  local_b   /* in */,
          int    local_n   /* in */,
          float  h         /* in */) {

    float integral;   /* Store result in integral  */
    float x;
    int i;

    float f(float x); /* function we're integrating */

    integral = (f(local_a) + f(local_b))/2.0;
    x = local_a;
    for (i = 1; i <= local_n-1; i++) {
        x = x + h;
        integral = integral + f(x);
    }
    integral = integral*h;
    return integral;
} /*  Trap  */


float f(float x) {
    float return_val;
    /* Calculate f(x). */
    /* Store calculation in return_val. */
    return_val = x*x;
    return return_val;
} /* f */



void Get_data3(
	      float*   a_ptr    /*out*/,
	      float*   b_ptr    /*out*/,
	      int*     n_ptr    /*out*/,
	      int      my_rank  /*in*/){
/*	if(my_rank == 0){
		printf("Enter a, b, and n\n");
		scanf("%f %f %d",a_ptr, b_ptr, n_ptr);
	}
*/
	char pack_buf[100];
	int position = 0;
	MPI_Pack(a_ptr, 1, MPI_DOUBLE, pack_buf, 100, &position, MPI_COMM_WORLD);
	MPI_Pack(b_ptr, 1, MPI_DOUBLE, pack_buf, 100, &position, MPI_COMM_WORLD);
	MPI_Pack(n_ptr, 1, MPI_INT, pack_buf, 100, &position, MPI_COMM_WORLD);

	MPI_Bcast(pack_buf, 100, MPI_PACK, 0, MPI_COMM_WORLD);
    
	MPI_Unpack( pack_buf, 100,  &position, a_ptr, 1, MPI_DOUBLE, MPI_COMM_WORLD);

    	MPI_Unpack( pack_buf, 100,  &position, b_ptr, 1, MPI_DOUBLE, MPI_COMM_WORLD);
   
       	MPI_Unpack( pack_buf, 100,  &position, n_ptr, 1, MPI_INT, MPI_COMM_WORLD);


	
}/*Get_data3*/










