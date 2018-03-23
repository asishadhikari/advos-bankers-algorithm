#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
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


void *customer_entry_point(void *customer_num);
//customer function prototypes: return 0 if success else -1
int request_resources(int customer_num, int request[]);
int release_resources(int customer_num, int release[]);
int is_need(int cust_id);

//mutexes for synchronisation across threads
pthread_mutex_t *l_available;
pthread_mutex_t *l_maximum; 
pthread_mutex_t *l_allocation;
pthread_mutex_t *l_need;

int main(int argc, char**argv){

	//validate resource instance user input
	if (argc-1!=NUMBER_OF_RESOURCES){
		printf("Invalid Number of Resources!! make sure %d resources\n",NUMBER_OF_RESOURCES);
		return -1;
	}else{
		for (int i = 1; i < argc; ++i)
		{
			if (atoi(argv[i])<1){
				printf("%d\n",atoi(argv[i]) );
				printf("Resource instance must be at least 1 exiting...\n");
				return -1;
			}
		}
	}
	//populate resources into available
	for (int i = 1; i < NUMBER_OF_RESOURCES+1; ++i)
		available[i]=atoi(argv[i]);

	l_need = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	l_allocation = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	l_maximum = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	l_available = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(&l_available[0],NULL);
	pthread_mutex_init(&l_allocation[0],NULL);
	pthread_mutex_init(&l_need[0],NULL);
	pthread_mutex_init(&l_maximum[0],NULL);

	pthread_t* customers = (pthread_t*)malloc(sizeof(pthread_t)*NUMBER_OF_CUSTOMERS);

	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
	{
		int *id = (int*)malloc(sizeof(int));
		*id = i;
		pthread_create(&customers[i],NULL,customer_entry_point,(void*)id);
	}

	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
		pthread_join(customers[i],NULL);
	}

	return 0;
}
int has_need(int cust_id){
	int counter =0;
	for (int i = 0; i < NUMBER_OF_RESOURCES; ++i)
		if(need[cust_id][i]!=0) counter++;
	int f = (counter==0)?1:0;
	return f;
}

int request_resources(int customer_num, int request[]){

}

int release_resources(int customer_num, int release[]){

}

void *customer_entry_point(void *customer_num){
	int cust_id = *((int*)customer_num);
	printf("%d\n",cust_id );
	//seed random number
	struct timeval start; 
	gettimeofday(&start, NULL);
	srand(start.tv_usec);  
	while(1){
		//populate need and max
		for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i)
		{
			int temp=(rand())%(available[i]+1);  //upto max resource requestable
			need[cust_id][i]= temp;
			maximum[cust_id][i] = temp;
		}
		
		//while need is >0, keep requesting randomly
		while(has_need(cust_id)){
			
		}

	
	}

}