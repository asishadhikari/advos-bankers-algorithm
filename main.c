#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
/* these may be any values >= 0 */
#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3
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

//customer functions
void *customer_entry_point(void*);
int request_resources(int customer_num, int* request);
int release_resources(int);
int has_need(int cust_id);
int is_safe();

//helper functions
int has_need(int cust_id); //if need==0 for cust_id
int is_leq(int*a, int*b);  //if a[]<=b[] 

//mutex for synchronisation across threads
pthread_mutex_t l_available;

int main(int argc, char**argv){
	//validate resource instance user input
	if (argc-1!=NUMBER_OF_RESOURCES){
		printf("Invalid Number of Resources!! make sure %d resources\n",NUMBER_OF_RESOURCES);
		return -1;
	}else{
		for (int i = 1; i < argc; i++)
		{
			if (atoi(argv[i])<1){
				printf("%d\n",atoi(argv[i]) );
				printf("Resource instance must be at least 1 exiting...\n");
				return -1;
			}
		}
	}
	//populate resources into available
	for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
		available[i]=atoi(argv[i+1]);
	pthread_mutex_init(&l_available,NULL);
	pthread_t* customers = (pthread_t*)malloc(sizeof(pthread_t)*NUMBER_OF_CUSTOMERS);
	//start customer threads
	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
	{
		int *id = (int*)malloc(sizeof(int));
		*id = i;
		pthread_create(&customers[i],NULL,customer_entry_point,(void*)id);
	}

	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
		pthread_join(customers[i],NULL);

	return 0;
}

int is_safe(){
	//since only thread with lock can enter here, no race conditions occur
	int* work = (int*)malloc(sizeof(int)*NUMBER_OF_RESOURCES);
	int* finish = (int*)malloc(sizeof(int)*NUMBER_OF_CUSTOMERS);
	int done;
	//initialise work to available
	for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
		work[i]=available[i];
	//initialise finish to false
	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
		finish[i] = 0;
	int count = 0;
	do{
		//find index for which need <= work and finish[i] == false
		for (int i = 0; i <NUMBER_OF_CUSTOMERS; i++)
		{
			if (finish[i]==0 && is_leq(need[i],work)){
				for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
					work[j]+= allocation[i][j];
				finish[i]=1;
			}else{
				done=0;
				for (int j = 0; j < NUMBER_OF_CUSTOMERS; j++)
					done+=finish[j];
				if (done==NUMBER_OF_CUSTOMERS) return 1; 
			}
		}
		count++;
	}while(count<NUMBER_OF_CUSTOMERS);

	free(work);
	free(finish);
	return 0;

}
int has_need(int cust_id){
	int counter =0;
	for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
		if(need[cust_id][i]!=0) counter++;
	counter = (counter==0)?0:1;
	return counter;
}

//is less than equal 
int is_leq(int *a, int *b){
	for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
		if (a[i]>b[i]) return 0;
	return 1;
}

int request_resources(int cust_id, int* request){
	//timestamp, customer id, # requested resource type 1, # requested resource type 2, ...
	pthread_mutex_lock(&l_available);
	printf(" %lu, %d, ",time(NULL),cust_id);
	for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
		printf(" %d requested resource type %d ",request[i],i);
	printf("\n");
	if (!is_leq(request,need[cust_id])||!is_leq(request,available)){
		printf(" %lu, %d, request denied\n",time(NULL),cust_id);
		pthread_mutex_unlock(&l_available);
		return -1;
	} 
	
	//resource request algorithm passed
	for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
		available[i]-= request[i];
		allocation[cust_id][i] += request[i];
		need[cust_id][i]-= request[i];
	}
	if(is_safe()){
		//timestamp, customer id, request satisfied (or request denied)
		printf(" %lu, %d, request satisfied\n",time(NULL),cust_id);
		pthread_mutex_unlock(&l_available);
		return 0;
	} 
	else{
		for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
		{
			available[i]+= request[i];
			allocation[cust_id][i] -= request[i];
			need[cust_id][i]+= request[i];
		}
	printf(" %lu, %d, request denied\n",time(NULL),cust_id);
	pthread_mutex_unlock(&l_available);
	return -1;
	}
}

int release_resources(int cust_id){
	pthread_mutex_lock(&l_available);
	for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
		available[i]+=allocation[cust_id][i];		
		allocation[cust_id][i]=0;
	}
	//timestamp, customer id, resource released
	printf(" %lu, %d, resource released\n",time(NULL),cust_id);
	pthread_mutex_unlock(&l_available);
	return 0;
}

void *customer_entry_point(void *customer_num){
	int cust_id = *((int*)customer_num);
	//seed random number
	struct timeval start; 
	gettimeofday(&start, NULL);
	srand(start.tv_usec);  
	while(1){
		//populate need and max
		for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
		{
			 //upto max resource requestable
			int temp=(rand())%(available[i]+1);
			need[cust_id][i]= temp;
			maximum[cust_id][i] = temp;
		}
		
		//while need is >0, keep requesting randomly
		int *request = (int*)malloc(sizeof(int)*NUMBER_OF_RESOURCES);
		int granted;
		while(has_need(cust_id)){
			//generate random request
			for (int i = 0; i <NUMBER_OF_RESOURCES; i++)
				request[i]= rand()%(need[cust_id][i]+1);
			granted = -1;
			while(granted==-1){
				granted =request_resources(cust_id,request);
				//sleep if request is denied
				if (granted==-1) usleep(rand()%11);
				//request fulfilled but has need
				else if(granted==0 && has_need(cust_id)) usleep(rand()%11);  
			}
		}
		free(request);
		//need fulfilled
		usleep(rand()%100);
		release_resources(cust_id);	
	}
}