#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
/* these may be any values >= 0 */
#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4
//MAX_NEED to bound n in need[]
#define MAX_NEED 100	  

//banker's data structures
/* the available amount of each resource */
int available[NUMBER_OF_RESOURCES];
/*the maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* the amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* the remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];


//customer function prototypes: return 0 if success -1 if failure
int request_resources(int customer_num, int request[]);
int release_resources(int customer_num, int release[]);


int main(int argc, char**argv){

	//handle resource instance input
	if (argc-1!=NUMBER_OF_RESOURCES){
		printf("Invalid Number of Resources!! make sure %d resources\n",NUMBER_OF_RESOURCES);
		return -1;
	}else{
		for (int i = 1; i < argc; ++i)
		{
			if (atoi(argv[i])<0){
				printf("%d\n",atoi(argv[i]) );
				printf("Negative resource instance given, exiting...\n");
				return -1;
			}
		}
	}

	pthread_t* customers = (pthread_t*)malloc(sizeof(pthread_t));
	
	
	return 0;
}