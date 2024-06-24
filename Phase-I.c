//Phase 1 Final mutex.

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>

//initializing board
int sudoko[9][9] = {
			{6,10,4,5,3,9,1,8,7},	//board with errors
			{5,1,9,7,2,8,6,3,4},
			{7,3,7,6,1,4,2,9,5},
			{1,4,3,8,6,5,7,2,9},
			{9,5,8,2,4,7,3,6,1},
			{7,6,2,3,9,1,4,5,8},
			{3,7,1,9,5,6,8,4,2},
			{4,9,6,1,8,2,5,7,3},
			{2,8,5,4,7,3,9,1,0}};

/*
int sudoko[9][9] ={
			{6,2,4,5,3,9,1,8,7},	//board without errors
			{5,1,9,7,2,8,6,3,4},
			{8,3,7,6,1,4,2,9,5},
			{1,4,3,8,6,5,7,2,9},
			{9,5,8,2,4,7,3,6,1},
			{7,6,2,3,9,1,4,5,8},
			{3,7,1,9,5,6,8,4,2},
			{4,9,6,1,8,2,5,7,3},
			{2,8,5,4,7,3,9,1,6}};
*/
pthread_t p1,p2,p3[9];	//p1 checks rows, p2 checks cols, p3 check sub matrix (3x3)		
pthread_mutex_t mut; 
int errors = 0;

int cancelled_threads =0;
long int cancel[9];
			
typedef struct
{
	int columns;
	int rows;
}para;			
			
			
			
//displays the board
void display(int sudo[9][9])
{
	printf("\n");
	for (int i = 0; i < 9; i++)
	{
		for (int j =0; j < 9; j++)
		{
			printf("%d ",sudo[i][j]);
		}
		printf("\n");
	}
}

void *checkRows(void *args)
{
	int localerror=0;
	printf("\n\n\n------The CheckRow is calculated using Phtread:%ld------",pthread_self());
	
	pthread_mutex_lock(&mut);
	for (int i =0; i < 9;i++)
	{
		int array[9];
		for (int x=0; x<9; x++)
			array[x] = 0;
			
		for (int j = 0; j < 9; j++)	//columns
		{
			int index = sudoko[i][j];
			
			if(sudoko[i][j] > 9 || sudoko[i][j] < 1)  //to check if the number is out of range
			{
				printf("\nThe number:%d at [%d,%d] is out of range in row:%d",sudoko[i][j],i+1,j+1,i+1);
				errors++;
				localerror++;
			}
			
			
			else if(array[index-1] == 1)	//to check if the number has already occured previously
			{ 				
				printf("\nThe number:%d at [%d,%d] is repeated before in this row number:%d",sudoko[i][j],i+1,j+1,i+1);
				errors++;
				localerror++;				
			}
			
			else
				array[index-1] = 1;			
			
		}
	}
	
	pthread_mutex_unlock(&mut);
	
	if(localerror == 0)
		return (void*)1;
	
	return (void *)0;	
	pthread_exit(NULL);
}

void *checkColumns(void *args)
{
	int localerror=0;
	printf("\n\n\n------The CheckColumns is calculated using Phtread:%ld------",pthread_self());


	pthread_mutex_lock(&mut);

	for (int i =0; i < 9;i++)
	{
		int array[9];
		for (int x=0; x<9; x++)
			array[x] = 0;
			
		for (int j = 0; j < 9; j++)	//columns
		{
			int index = sudoko[j][i];
			
			if(sudoko[j][i] > 9 || sudoko[j][i] < 1)  //to check if the number is out of range
			{
				printf("\nThe number:%d at [%d,%d] is out of range in column:%d",sudoko[j][i],j+1,i+1,i+1);
				errors++;
				localerror++;
			}
	
			
			else if(array[index-1] == 1) 			//to check if the number has already occured previously
			{	
				printf("\nThe number:%d at [%d,%d] is repeated before in this column number:%d",sudoko[j][i],j+1,i+1,i+1);				
				errors++;
				localerror++;
			}
	
			
			else
				array[index-1] = 1;			
			
		}
	}
	
	pthread_mutex_unlock(&mut);

	if(localerror == 0)
		return (void*)1;
	
	cancel[cancelled_threads++] = p1;
	pthread_cancel(p1);
	return (void *)0;	
	pthread_exit(NULL);	
}

void *check3x3(void *args)
{
	para *parameters = (para*) args;
	int rows = parameters->rows;
	int columns = parameters->columns;
	int localerror =0;
	printf("\n\n\n------The 3x3 is calculated using Phtread:%ld------",pthread_self());
	
	
	
	pthread_mutex_lock(&mut);
	int array[9];
	for (int i=0;i<9;i++)
		array[i] = 0;
	
	for (int i=rows; i< rows + 3 ;i++)
	{
		for (int j=columns; j<columns + 3;j++)
		{
			int index = sudoko[i][j];

			if(sudoko[i][j] > 9 || sudoko[i][j] < 1)  //to check if the number is out of range
			{
				printf("\nThe number:%d at [%d,%d] is out of range",sudoko[i][j],i+1,j+1);
				errors++;
				localerror++;
			}
	
			
			else if(array[index-1] == 1) 			//to check if the number has already occured previously
			{
				printf("\nThe number:%d at [%d,%d] is repeated before in 3x3 matrix",sudoko[i][j],i+1,j+1);				
				errors++;
				localerror++;
			}
	
			else
				array[index-1] = 1;				
		}
	}
	
	pthread_mutex_unlock(&mut);
	if(localerror==0)
		return(void*)1;
	
	return (void*)0;	
	pthread_exit(NULL);
}



int main()
{
	
	pthread_mutex_init(&mut,NULL);
	display(sudoko);


	void * rows_value;
	void * col_value;
	void * box1,*box2,*box3,*box4,*box5,*box6,*box7,*box8,*box9;	//return values

	pthread_create(&p1,NULL,checkRows,NULL);
	pthread_join(p1,&rows_value);

	pthread_create(&p2,NULL,checkColumns,NULL);
	pthread_join(p2,&col_value);
	
	int thread_no =0;	//used for iterating p3
	for (int i=0; i < 9; i++)
	{
		for (int j=0; j < 9; j++)
		{
			if(i % 3 == 0 && j % 3 == 0)
			{
				para *data = (para *) malloc(sizeof(para));	
				data->rows = i;
				data->columns = j;
				pthread_create(&p3[thread_no++], NULL, check3x3, data);
			}
		}
	}	
	
	//for (int i = 0; i < 9; i++)
	//	pthread_join(p3[i], NULL);
	int t=0;
	pthread_join(p3[t++],&box1);
	pthread_join(p3[t++],&box2);
	pthread_join(p3[t++],&box3);
	pthread_join(p3[t++],&box4);
	pthread_join(p3[t++],&box5);
	pthread_join(p3[t++],&box6);
	pthread_join(p3[t++],&box7);
	pthread_join(p3[t++],&box8);
	pthread_join(p3[t++],&box9);
	



	printf("\n\nNumber of Invalid Entries:%d (terms:%d)",errors,errors/3);

	if( (int)rows_value == 1 && (int)col_value ==1 && (int)box1 ==1 && (int)box2 ==1 && (int)box3 ==1 && (int)box4 ==1 && (int)box5 ==1 && (int)box6 ==1 && (int)box7 ==1 && (int)box8 ==1 && (int)box9 ==1 ||errors ==0)
		printf("\nSolution is valid for Sudoko\n");

	else
		printf("\nSodoko Soltuion is invalid\n");


	cancelled_threads--;
	printf("\nCancelled Threads are:-");
		printf("%ld\n",cancel[0]);
	
	
	



	
}			
                   



