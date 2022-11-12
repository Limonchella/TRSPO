#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#include<bits/stdc++.h>
#include "mpi.h"
#include <chrono>
using namespace std;


double piMeasure(double* pointsArray, int size) {
    int count = 0;
    for (int i = 0; i < 2 * size; i += 2) {
        if (pow(pointsArray[i], 2) + pow(pointsArray[i + 1], 2) <= 1)
            count += 1;
    }

    return (double)(count * 4) / size;
}




int main(int argc, char** argv) {
    srand(time(0));
    double response[2];
    int proccessCount;
    int pointsCount = 10000000;
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proccessCount);
    int PointsThisProc = rank * floor( pointsCount / (proccessCount-1));
    if (rank == 0) {
        double* pointsArray = new double[2 * pointsCount];
        for (int i = 0; i < 2 * pointsCount; i++) {
            pointsArray[i] = (double)rand() / RAND_MAX;
        }
        for (int i = 1; i < proccessCount; i++) {
            int size = 2 * i * floor( pointsCount / (proccessCount-1));
            double* array = new double[size];
            for (int j = 0; j < size; j++) {
                array[j] = pointsArray[j];
            }
            MPI_Send(array, size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            delete[] array;
        }
    }

    else {
        double* pointsArray = new double[2 * PointsThisProc];
        MPI_Recv(pointsArray, 2 * PointsThisProc, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();
        double pi = piMeasure(pointsArray, PointsThisProc / 2);
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> time_ = end - start;
        double duration = time_.count();
        response[0] = duration;
        response[1] = pi;
    }

    if (rank != 0) {
        MPI_Send(response, 2, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    else {
        for (int i = 1; i < proccessCount; i++) {
            MPI_Recv(response, 2, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
           
            cout << "Points: " << i * floor( pointsCount / (proccessCount-1)) << "\t| Pi = " << response[1] << "\t| Time: " << response[0] << "\n";
        }
    }

    MPI_Finalize();
    return 0;
}
