/* 
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
int value = 0;
MPI_Status status;
int countOfProcesses, currentProcess;

void sendValue(int value);
void receiveValue(int receiver);

int main(int* argc, char** argv)
{
	int sender = 0;		
	int receiver = 1;	
	
	if (sender == receiver) {
		cout << "You cannot send value by some process to itself!" << endl;
		return 0;
	}

	MPI_Init(argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);
	MPI_Comm_size(MPI_COMM_WORLD, &countOfProcesses);

	if (countOfProcesses - 1 <= sender || countOfProcesses - 1 <= receiver) {
		if (sender > receiver) {
		cout << "Please select a count of processes greater than " << sender + 1 << endl;
		}
		else {
			cout << "Please select a count of processes greater than " << receiver + 1 << endl;
		}
		return 0;
	}

	int value = 13;
	//MPI_Status status;

	// PART 1: from one to another
	/*if (currentProcess == sender) {
		sendValue(receiver, value);
	} 
	else {			
		receiveValue(receiver);
	}*/
	

	//PART 2: form all to all
	value = currentProcess;

	sendValue(currentProcess);

	for (int i = 0; i < countOfProcesses; i++) {
		if (i == currentProcess) {
			continue;
		}
		receiveValue(i);
	}

	cout << "Process " << currentProcess << " got " << value << endl;

	MPI_Finalize();
}

void sendValue(int value) {
	if (currentProcess == countOfProcesses - 1) {
		MPI_Send(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	else {
		MPI_Send(&value, 1, MPI_INT, currentProcess + 1, 0, MPI_COMM_WORLD);
	}
}

void receiveValue(int receiver) {
	MPI_Recv(&value, 1, MPI_INT, currentProcess == 0 ? countOfProcesses - 1 : currentProcess - 1, 0, MPI_COMM_WORLD, &status);

	if (currentProcess != receiver) {
		MPI_Send(&value, 1, MPI_INT, currentProcess == countOfProcesses - 1 ? 0 : currentProcess + 1, 0, MPI_COMM_WORLD);
	}
}
