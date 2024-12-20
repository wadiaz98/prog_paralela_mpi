#include <iostream>
#include <memory>

#include <mpi.h>

#define MATRIX_DIM 25

void imprimir_vector(std::string txt, double* v, int size)
{
    std::printf("%d","[");
    for(int i = 0; i < size; i++)
    {
        std::printf("%f", v[i]);
        if(i < size - 1)
        {
            std::printf("%s", ", ");
        }
    }
}

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank, nprocs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    int rows_per_rank;  //N° de filas a enviar a cada rank
    int rows_alloc = MATRIX_DIM;     //tamaño ajustado de la matriz para que el # de filas sea divisibale para nprocs
    int padding = 0;        //n° de filas que se agregan a la matriz para que rows_alloc sea divisible por nprocs

    if(MATRIX_DIM % nprocs != 0)
    {
        //El número de filas no es divisible por el número de procesos
        rows_alloc = std::ceil(((double)MATRIX_DIM / nprocs) * nprocs);
        padding = rows_alloc - MATRIX_DIM;
    }

    //buffers
    //b = A*x
    std::unique_ptr<double[]> A;    //Solo RANK_0
    std::unique_ptr<double[]> b;    //Solo RANK_0
    std::unique_ptr<double[]> x = std::make_unique<double[]>(MATRIX_DIM); //make_unique es una forma de inicializar un puntero inteligente y reserva memoria

    //bi = Ai*x, donde Ai es una matriz de dimension rows_per_rank x MATRIX_DIM -> 7*25
    /*
     *A_0 = [
            f0
            f1
            f2
            f3
            f4
            f5
            f6
            ]

        A_1 = [
            f7
            f8
            f9
            f10
            f11
            f12
            f13
            ]

        A_2 = [
            f14
            f15
            f16
            f17
            f18
            f19
            f20
            ]

        A_3 = [
            f21
            f22
            f23
            f24
            f25->padding
            f26->padding
            f27->padding
            ]
     *
     */
    std::unique_ptr<double[]> A_local;
    std::unique_ptr<double[]> b_local;

    rows_per_rank = rows_alloc / nprocs;

    if(rank==0)
    {
        std::printf("Dimension=%d, rows_alloc=%d, padding=%d, rows_per_rank=%d\n", MATRIX_DIM, rows_alloc, padding, rows_per_rank);
        A = std::make_unique<double[]>(rows_alloc * MATRIX_DIM);
        b = std::make_unique<double[]>(rows_alloc);

        //inicializar matriz A, vector x
        for(int i = 0; i < MATRIX_DIM; i++)
        {
            for(int j = 0; j < MATRIX_DIM; j++)
            {
                int index = i*MATRIX_DIM + j;
                A[index] = i;

            }
        }

        for(int i = 0; i<MATRIX_DIM; i++)
        {
            x[i] = 1;
        }

    }


    //Inicializar matrices locales
    A_local = std::make_unique<double[]>(rows_per_rank * MATRIX_DIM);
    b_local = std::make_unique<double[]>(rows_per_rank);

    //imprimir vector 'x'
    // std::string txt = std::printf("RANK_%d before:", rank);
    // imprimir_vector(txt, x.get(), MATRIX_DIM);
    //
    // MPI_Bcast(x.get(), MATRIX_DIM, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    //
    // txt = std::printf("RANK_%d affter:", rank);
    // imprimir_vector(txt, x.get(), MATRIX_DIM);

    MPI_Finalize();

    return 0;
}