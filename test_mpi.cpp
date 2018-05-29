#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[]){
	// Initializes MPI
	int task_id, num_tasks, n;
	MPI_Init(&argc, &argv);

  	MPI_Comm_rank(MPI_COMM_WORLD, &task_id);
  	MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);

  	MPI_Status status;

  	int* numbers;

  	if(task_id == 0){
  		cout << "proc " << task_id << " started" << endl;
  		numbers = new int[10];
  		for(int i = 0; i < 10; i++)
  			numbers[i] = i;
  		MPI_Send(NULL, 0, MPI_INT, 1, 42, MPI_COMM_WORLD);
  	}
  	else {
  		numbers = new int[20];
  		cout << "proc " << task_id << " started" << endl;
  		MPI_Recv(numbers, 20, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  		MPI_Get_count(&status, MPI_INT, &n);
  		cout << "proc " << task_id << " received " << n << " ints from " << status.MPI_SOURCE << " with tag " << status.MPI_TAG << endl;
  		for(int i = 0; i < n; i++)
  			cout << numbers[i] << " ";
  		cout << endl;
  	}

  	MPI_Finalize();
}