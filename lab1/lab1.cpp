﻿/* 
13.Топология процессов – «кольцевой линейный массив» из n(n – любое
	заданное значение) процессов(рис. 5).С помощью функций обменов
	осуществить :
- передачу данных от процесса № i процессу № j, где 0 ≤ i, j < n;
- передачу различных данных(каждому процессу свое данное) от каждого процесса каждому.
*/

// countOfProcesses - кол-во процессов 
// currentProcess - номер процесса начинается с 0

#include <iostream>
#include <stdio.h>
#include "mpi.h"
#include <cmath>

using namespace std;

int countOfProcesses;
const int sender = 6;
const int receiver = 3;

bool ifProcessWillBeScipped(int countOfSkippedProcesses, int indexOfProcess);

int main(int* argc, char** argv)
{
	int value = 0;
	MPI_Status status;
	int currentProcess;
	if (sender == receiver) {
		cout << "You cannot send value by some process to itself!" << endl;
		return 0;
	}

	MPI_Init(argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);
	MPI_Comm_size(MPI_COMM_WORLD, &countOfProcesses);

	const int countOfSkippedProcesses = receiver > sender ?
		countOfProcesses - (receiver - sender) - 1 :
		(sender - receiver) - 1;

	if (countOfProcesses - 1 < sender || countOfProcesses - 1 < receiver) {
		if (sender > receiver) {
		cout << "Please select a count of processes greater than " << sender << endl;
		}
		else {
			cout << "Please select a count of processes greater than " << receiver << endl;
		}
		return 0;
	}

	// PART 1: from one to another
	value = 4;
	if (currentProcess == sender) {
		MPI_Send(&value, 1, MPI_INT, (currentProcess == countOfProcesses - 1) ? 0 : currentProcess + 1, 0, MPI_COMM_WORLD);
	}
	else {
		if (!ifProcessWillBeScipped(countOfSkippedProcesses, currentProcess)) {
			MPI_Recv(&value, 1, MPI_INT, (currentProcess == 0) ? countOfProcesses - 1 : currentProcess - 1, 0, MPI_COMM_WORLD, &status);
			cout << "Process " << currentProcess << " got " << value << endl;

			if (currentProcess != receiver && !ifProcessWillBeScipped(countOfSkippedProcesses, (currentProcess == countOfProcesses - 1) ? 0 : currentProcess + 1)) {
				MPI_Send(&value, 1, MPI_INT, (currentProcess == countOfProcesses - 1) ? 0 : currentProcess + 1, 0, MPI_COMM_WORLD);
			}

		}
	}
	

	//PART 2: form all to all
	//value = currentProcess;

	//currentProcess == 0 ? receiveValue(countOfProcesses - 1) : receiveValue(currentProcess - 1);
	//sendValue(currentProcess);

	///*for (int i = 0; i < countOfProcesses; i++) {
	//	if (i == currentProcess) {
	//		continue;
	//	}
	//}
	//*/	


	MPI_Finalize();
}

bool ifProcessWillBeScipped(int countOfSkippedProcesses, int indexOfProcess)
{
	if (countOfSkippedProcesses != 0) {
		for (int i = 1; i <= countOfSkippedProcesses; i++) {
			if (indexOfProcess == (receiver + i) % countOfProcesses) {
				return true;
			}
		}
	}
	return false;
}
