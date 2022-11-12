#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <mpi.h>

//Youssef Ahmed AbdelHafez 20180337
//Khaled Samir             20180095

int main ( int argc , char * argv []){
    int n , myid , numprocs , i;
    double localSum, globalSum;
    MPI_Init(& argc ,& argv );

    MPI_Comm_size( MPI_COMM_WORLD ,& numprocs );
    MPI_Comm_rank( MPI_COMM_WORLD ,& myid );
    n = 0;

    for (;;) {
        if ( myid == 0) {
            printf ("Enter the number of The limit of the function : (write \"0\" to quit) ");
            scanf ("%d" ,&n );
        }
        MPI_Bcast(&n, 1, MPI_INT , 0 , MPI_COMM_WORLD );
        if ( n == 0)
            break ;
        else {
            localSum = 0.0;
            #pragma omp parallel for reduction(+:localSum) private(i)
            for ( i = myid + 1; i <= n ; i += numprocs) {
                double value = (double)1/ (double)pow(2,i);
                localSum += value;
                printf("Rank of %d, with Index (%d), from Thread %d, Give Value: %.16f \n", myid,i, omp_get_thread_num(),value);
            }

            MPI_Reduce(&localSum , &globalSum , 1 , MPI_DOUBLE , MPI_SUM , 0 , MPI_COMM_WORLD );
            if ( myid == 0) {
                printf("The Expected Value should be equal: 1\n");
                printf ("The Result is approximately : %.16f Error is: %.16f\n", globalSum , 1.0 - globalSum);
            }
        }
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}
