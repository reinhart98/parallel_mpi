#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_VALUE 10
#define AROW 3
#define ACOL 3

int proc_map(int i, int size){
	size = size-1;
	int r=(int)ceil((double)AROW/(double)size);
	int proc = i/r;
	return proc+1;
}

int main(int argc, char* argv[]){
	int numofproc,rank,namelen,size;
	MPI_Init(&argc,&argv);
	MPI_Status Stat;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	//char processorname[MPI_MAX_PROCESSOR_NAME];
	//MPI_Get_processorname(processorname, &namelen);

	if(rank==0){
	int a[AROW];
	int b[ACOL];
	int c[AROW];

	srand(time(NULL));
	for(int i=0;i<AROW;i++){
		for(int j=0;j<ACOL;j++){
			if(j==0)a[i]=rand()%MAX_VALUE;
			if(i==0)b[j]=rand()%MAX_VALUE;
		}
	}

	/*print matrix*/
	printf("vector A:\n");
	for(int i=0;i<AROW;i++){
	printf("%3d",a[i]);
	
	}
	printf("\n vector B:\n");
	for(int i=0;i<ACOL;i++){
	printf("%3d",b[i]);
	printf("\n");
	}
	printf("\n");

	//sending a value to other process
	for(int j=1;j<size;j++){
	MPI_Send(b,ACOL,MPI_INT,j,99,MPI_COMM_WORLD);
	}
	for(int i=0;i<AROW;i++){
	int pros = proc_map(i,size);
	MPI_Send(&a[i],AROW,MPI_INT,pros,(100*(i+1)),MPI_COMM_WORLD);
	}

	for(int i=0;i<AROW;i++){
	int source_processor=proc_map(i,size);
	MPI_Recv(&c[i],1,MPI_INT,source_processor,i,MPI_COMM_WORLD,&Stat);
	printf("P%d : c[%d]t=%dn",rank,i,c[i]);
	}

	}
	else{
	int b[ACOL];
	MPI_Recv(b,ACOL,MPI_INTEGER,0,99,MPI_COMM_WORLD,&Stat);

	for(int i=0;i<AROW;i++){
	int pros=proc_map(i,size);
	if(rank==pros){
		int buffer[ACOL];
		MPI_Recv(buffer,0,MPI_INT,0,(100*(i+1)),MPI_COMM_WORLD,&Stat);
		int sum=0;
		for(int j=0;j<ACOL;j++){
			sum=sum+(buffer[0]*b[j]);
		}
		MPI_Send(&sum,1,MPI_INT,0,i,MPI_COMM_WORLD);
		
	}
	}

}
MPI_Finalize();
return 0;
}
