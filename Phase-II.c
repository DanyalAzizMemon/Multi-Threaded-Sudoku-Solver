//Phase 2 Final.

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

/*
int sudoko[9][9] = {				//4 errors in sudoko
			{6,10,4,5,3,9,1,8,7},	//value of 10 at index 2
			{5,1,9,7,2,8,6,3,4},
			{8,3,7,6,1,4,2,9,5},
			{1,4,3,8,6,5,7,2,9},
			{9,5,8,2,4,7,3,6,1},
			{8,6,2,3,9,1,4,5,8},	//8 at start
			{3,7,1,9,5,6,8,4,2},	
			{5,9,6,1,8,2,5,7,3},	 //5 at start
			{2,8,5,4,7,8,9,1,6}};	//value of 8 at index 6
*/


int sudoko[9][9] = {				//5 errors in sudoko
			{6,10,4,5,3,9,1,8,7},	//value of 10 at index 2
			{5,1,9,7,2,8,6,3,4},
			{8,3,7,6,1,4,2,9,5},
			{1,4,3,8,6,5,7,2,9},
			{9,5,8,2,4,7,3,6,9},	//9 at index 9
			{8,6,2,3,9,1,4,5,8},	//8 at start
			{3,7,1,9,5,6,8,4,2},	
			{5,9,6,1,8,2,5,7,3},	 //5 at start
			{2,8,5,4,7,8,9,1,6}};	//value of 8 at index 6



/*int sudoko[9][9] ={
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


	
			
int valid_check[27] = {0};	//this would check where the error occurs 
int errors = 0;			//number of errors in sudoko
int orignal[9][9];		//for copying the orignal board before making changing
int lock=0;
pthread_mutex_t mut;
int swaps=0;
int threads_created = 0;
	
	
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
			printf("%d ",sudo[i][j]);
		printf("\n");
	}
}


void copy()
{
	for(int i=0; i<9; i++)
		for(int j=0; j<9; j++)
			orignal[i][j] = sudoko[i][j];
}


void *checkRows(void *args)
{
	para *parameters = (para*) args;
	int rows = parameters->rows;
	int columns = parameters->columns;
	int error_detected = 0;

	printf("\n\n\n------The CheckRow is calculated using Phtread:%ld------",pthread_self());

		int array[9];
		for (int x=0; x<9; x++)
			array[x] = 0;

	
	for (int i =0; i < 9;i++)
	{
			
		int index = sudoko[rows][i];
			
		if(sudoko[rows][i] > 9 || sudoko[rows][i] < 1)  //to check if the number is out of range
		{
			printf("\nThe number:%d at [%d,%d] is out of range in row:%d",sudoko[rows][i],rows+1,i+1,rows+1);
			errors++;
			error_detected =1;
		}
		
		
		else if(array[index-1] == 1)	//to check if the number has already occured previously
		{ 				
			printf("\nThe number:%d at [%d,%d] is repeated before in this row number:%d",sudoko[rows][i],rows+1,i+1,rows+1);
			errors++;
			error_detected =1;				
		}
			
		else
			array[index-1] = 1;			
			
	}
	if(error_detected)
		valid_check[9 +rows] = 1;
	
	
	pthread_exit(NULL);
}



void *correctRows(void *args)
{
	printf("\n\n------------------Correct Row Function is working using thread:%ld------------------",pthread_self());
	pthread_mutex_lock(&mut);
	for(int i=0; i<9; i++)
	{
		int array[9] ={0};
		int errors_array[9] = {0};		//stores the index where the error occured (j)
		int errors_iterator =0;			//used for iterating the errors array
		int error_detected=0;
		for(int j=0; j<9; j++)
		{
			int index = sudoko[i][j];
			if(index > 9 || index < 1)  //to check if the number is out of range
			{
				printf("\nThe number:%d at [%d,%d] is out of range in row:%d",sudoko[i][j],i+1,j+1,i+1);
				
				errors_array[errors_iterator++] = j;
				error_detected++;

			}
			
			else if(array[index-1] == 1)	//to check if the number has already occured previously
			{ 				
				printf("\nThe number:%d at [%d,%d] is repeated before in this row number:%d",sudoko[i][j],i+1,j+1,i+1);
				errors_array[errors_iterator++] = j;
				error_detected++;
			}
			
			else
				array[index-1] = 1;	
			
		}

		errors_iterator--;		
		while(error_detected > 0)	//while loop works till all the errors in rows are fixed
		{
			for(int k=0; k<9; k++)
			{
				if(array[k] == 0)	//if a value is not available in row
				{
					sudoko[i][errors_array[errors_iterator]] = k+1;	//place it at the place where the error occured
					printf("\nValue placed:%d at index:[%d][%d]",k+1,i+1,errors_array[errors_iterator]+1);
					errors_iterator--; 
					error_detected--;
					swaps++;
					threads_created++;
					
				}
			}			
		}
		
	}
	pthread_mutex_unlock(&mut);
}


void *correctColumns(void *args)
{
	printf("\n\n------------------Correct Column Function is working using thread:%ld------------------",pthread_self());
	
	
	pthread_mutex_lock(&mut);
	for(int i=0; i<9; i++)
	{
		int array[9] ={0};
		int errors_array[9] = {0};		//stores the index where the error occured (j)
		int errors_iterator =0;			//used for iterating the errors array
		int error_detected=0;
		for(int j=0; j<9; j++)
		{
			int index = sudoko[j][i];
			if(index > 9 || index < 1)  //to check if the number is out of range
			{
				printf("\nThe number:%d at [%d,%d] is out of range in column:%d",sudoko[j][i],j+1,i+1,i+1);
				
				errors_array[errors_iterator++] = j;
				error_detected++;

			}
			
			else if(array[index-1] == 1)	//to check if the number has already occured previously
			{ 				
				printf("\nThe number:%d at [%d,%d] is repeated before in this column number:%d",sudoko[j][i],j+1,i+1,i+1);
				errors_array[errors_iterator++] = j;
				error_detected++;
			}
			
			else
				array[index-1] = 1;	
			
		}

		errors_iterator--;		
		while(error_detected > 0)	//while loop works till all the errors in rows are fixed
		{
			for(int k=0; k<9; k++)
			{
				if(array[k] == 0)	//if a value is not available in row
				{
					sudoko[errors_array[errors_iterator]][i] = k+1;	//place it at the place where the error occured
					printf("\nValue placed:%d at index:[%d][%d]",k+1,errors_array[errors_iterator]+1,i+1);
					errors_iterator--; 
					error_detected--;
					swaps++;
					threads_created++;		
				}
			}			
		}
		
	}
	pthread_mutex_unlock(&mut);
}


void *checkColumns(void *args)
{
	para *parameters = (para*) args;
	int rows = parameters->rows;
	int columns = parameters->columns;
	int error_detected = 0;


	printf("\n\n\n------The CheckColumns is calculated using Phtread:%ld------",pthread_self());

		int array[9];
		for (int x=0; x<9; x++)
			array[x] = 0;


	for (int i =0; i < 9;i++)
	{
			
		int index = sudoko[i][columns];
			
		if(sudoko[i][columns] > 9 || sudoko[i][columns] < 1)  //to check if the number is out of range
		{
			printf("\nThe number:%d at [%d,%d] is out of range in column number: %d",sudoko[i][columns],i+1,columns+1,columns+1);
			errors++;
			error_detected = 1;
		}
	
			
		else if(array[index-1] == 1) 			//to check if the number has already occured previously
		{	
			printf("\nThe number:%d at [%d,%d] is repeated before in this column number:%d",sudoko[i][columns],i+1,columns+1,columns+1);				
			errors++;
			error_detected = 1;
		}
	
			
		else
			array[index-1] = 1;			
	}
	if(error_detected)
		valid_check[18 + columns] = 1;	
	pthread_exit(NULL);	
}

void *check3x3(void *args)
{
	int error_detected =0;
	para *parameters = (para*) args;
	int rows = parameters->rows;
	int columns = parameters->columns;
	
	printf("\n\n\n------The 3x3 is calculated using Phtread:%ld------",pthread_self());
	
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
				error_detected = 1;					
			}
	
			
			else if(array[index-1] == 1) 			//to check if the number has already occured previously
			{
				printf("\nThe number:%d at [%d,%d] is repeated before in 3x3 matrix",sudoko[i][j],i+1,j+1);				
				errors++;
				error_detected = 1;					
			}
	
			else
				array[index-1] = 1;				
		}
	}
	
	if(error_detected)
		valid_check[rows + columns/3] = 1;		
	pthread_exit(NULL);
}


void *correct3x3(void *args)
{

	printf("\n\n------------------Correct 3x3 block is working using thread:%ld------------------",pthread_self());
	
	para *parameters = (para*) args;
	int rows = parameters->rows;
	int columns = parameters->columns;
	

	int array[9] = {0};
	int errors_array_i[9] = {0};		//stores the index where the error occured (j)
	int errors_iterator_i =0;		//used for iterating the errors array
	
	
	int errors_iterator_j =0;
	int errors_array_j[9] = {0};
	
	int error_detected=0;
	
	
	

	
	pthread_mutex_lock(&mut);		
	for (int i=rows; i< rows + 3 ;i++)
	{
		for (int j=columns; j<columns + 3;j++)
		{
			int number = sudoko[i][j];
			if(number > 9 || number < 1)  //to check if the number is out of range
			{
				printf("\nThe number:%d at [%d,%d] is out of range",sudoko[i][j],i+1,j+1);
				error_detected++;
				errors_array_i[errors_iterator_i++] = i;
				errors_array_j[errors_iterator_j++] = j;
			}
	
			
			else if(array[number-1] == 1) 			//to check if the number has already occured previously
			{
				printf("\nThe number:%d at [%d,%d] is repeated before in 3x3 matrix",sudoko[i][j],i+1,j+1);				
				error_detected++;					
				errors_array_i[errors_iterator_i++] = i;
				errors_array_j[errors_iterator_j++] = j;

			}
			else
				array[number-1] = 1;				
			 
		}	
	}
	
		
	errors_iterator_j--;
	errors_iterator_i--;
	
	while(error_detected > 0)
	{
		for(int k =0; k <9; k++)
		{
			if(array[k] == 0)
			{
				sudoko[errors_array_i[errors_iterator_i]] [errors_array_j[errors_iterator_j]] = k+1;
				printf("\nValue placed:%d at index[%d][%d]",k+1,errors_array_i[errors_iterator_i]+1,errors_array_j[errors_iterator_j]+1);
				errors_iterator_i--;
				errors_iterator_j--;
				error_detected--;
				swaps++;
				threads_created++;
			}
		}
	
	}
	pthread_mutex_unlock(&mut);	
	pthread_exit(NULL);
	



}

int main()
{
	pthread_mutex_init(&mut,NULL);
	copy();
	display(orignal);
	pthread_t t[27];
	
	int thread_no=0;	//iterating threads in for loop
	

	for (int rows =0; rows < 9; rows++)
	{
		for (int columns=0; columns < 9; columns++)
		{
			if((rows % 3 == 0) && (columns % 3 == 0))
			{
				para *data = (para *) malloc(sizeof(para));
				data->rows = rows;
				data->columns = columns;
				pthread_create(&t[thread_no++], NULL , check3x3, data);	
			}
			
			if(rows == 0)
			{
				para *data = (para *) malloc(sizeof(para));
				data->rows = rows;
				data->columns = columns;
				pthread_create(&t[thread_no++], NULL , checkColumns, data);	
			}
			
			if(columns == 0)
			{
				para *data = (para *) malloc(sizeof(para));
				data->rows = rows;
				data->columns = columns;
				pthread_create(&t[thread_no++], NULL , checkRows, data);				
			}
		}
	}

	for (int i=0; i < 27; i++)	//joining together to wait till they all complete
		pthread_join(t[i],NULL);


	if(errors == 0)
	{
		printf("\nThe Sudoko Solution is valid and does not require any swapping\n");
		return 0;
	}


	thread_no=0;	//reseting iterator
	pthread_t p[27];
	printf("\n\nNumber of Invalid Entries:%d (terms:%d)",errors,errors/3);
	
	
	
	for(int i =0; i < 27;i++)									
	{
		if(valid_check[i]==1)
		{
						
			if(i >= 9 && i < 18)
				printf("\nError in Row:%d\n",i-9+1);			
				
			else if(i>=18)
				printf("\nError in Column:%d\n",i-18+1);
			
			else if(i < 9)
				printf("\nError in box:%d\n",i+1);
		}
	}

	for(int i=0; i<9; i++)
	{
		for(int j=0; j<9; j++)
		{
			if(i%3 == 0 && j%3 ==0)
			{
				para *data = (para *) malloc(sizeof(para));	
				data->rows = i;
				data->columns = j;
				
				pthread_create(&p[thread_no],NULL,correct3x3,data);
				pthread_join(p[thread_no++],NULL);			
			}
			
			if(i ==0)
			{
				pthread_create(&p[thread_no],NULL,correctColumns,NULL);
				pthread_join(p[thread_no++],NULL);
			}
			
			if(j==0)
			{
				pthread_create(&p[thread_no],NULL,correctRows,NULL);
				pthread_join(p[thread_no++],NULL);
			}
		}

	}

		//running this once again to check if any row is remaining
				pthread_t th[5];
				thread_no =0;
				
				
				pthread_create(&th[thread_no],NULL,correctRows,NULL);
				pthread_join(th[thread_no++],NULL);

				pthread_create(&th[thread_no],NULL,correctColumns,NULL);
				pthread_join(th[thread_no++],NULL);

				pthread_create(&th[thread_no],NULL,correctRows,NULL);
				pthread_join(th[thread_no++],NULL);

				pthread_create(&th[thread_no],NULL,correctColumns,NULL);
				pthread_join(th[thread_no++],NULL);

	


	printf("\n\n\nOriginal Sudoko\n\n");
	display(orignal);
	printf("\n\nSolved Sudoko\n\n");
	display(sudoko);

	printf("\n\nNumber of swaps made to correct the output: %d",swaps);
	printf("\nThreads required to rectify problems: %d\n",threads_created);
	pthread_mutex_destroy(&mut);
		
}			
                   



