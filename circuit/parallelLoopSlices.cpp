/* parallelLoopSlices.cpp
 * ... illustrates the parallel for loop pattern in MPI 
 *	in which processes perform the loop's iterations in 'slices' 
 *	(simple, and useful when loop iterations do not access
 *	 memory/cache locations) ...
 */

#include <iostream>
#include <mpi.h>

using std::cout, std::endl;

int main(int argc, char** argv) {
	const int REPS = 8;
	int id = -1, numProcesses = -1, i = -1;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

	for (i = id; i < REPS; i += numProcesses) {
		cout << "Process " << id << " is performing iteration " << i << endl;
	}

	MPI_Finalize();
	return 0;
}


