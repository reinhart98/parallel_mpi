#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
	char mass[30];
	int rank;
	MPI_Status stats;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	printf("proses pada rank : %d \n",rank);
	
	if(rank==0){
		strcpy(mass,"halo ini dr rank 0");
		printf("data dikirim dari rank 0 : %s \r\n",mass);
		MPI_Send(&mass,strlen(mass)+1,MPI_CHAR,1,99,MPI_COMM_WORLD);	
	}
	else if(rank==1){
		MPI_Recv(&mass,30,MPI_CHAR,0,99,MPI_COMM_WORLD,&stats);	
		printf("data diterima dari rank 0 : %s \r\n",mass);
	}
	MPI_Finalize();
	return 0;
	
}