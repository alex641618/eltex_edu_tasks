#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int init_shared_memory( void **obj, int size, char *filename, int proj_id ) {
	key_t key;
	int mem_id;

	if(obj==NULL || size<=0 || filename==NULL) return -1;
		
	key = ftok(filename, proj_id);
	if(key==-1) return -1;

	mem_id=shmget(key, size, IPC_CREAT);
	if(mem_id==-1) return -1;		
	
	*obj=shmat(mem_id,NULL,0);
	if( *obj==(void *)-1 ) return -1;		
	
	memset( *obj, 0, size );
	
	return 0;
	
}

int get_shared_memory( void **obj, int size, char *filename, int proj_id ) {
	key_t key;
	int mem_id;

	if(obj==NULL || size<=0 || filename==NULL) return -1;

	key = ftok(filename, proj_id);
	if(key==-1) return -1;
	
	mem_id=shmget(key, size , 0);
	if(mem_id==-1) return -1;		
	
	*obj=shmat(mem_id,NULL,0);
	if( *obj==(void *)-1 ) return -1;		
	
	return 0;
	
}

int detach_shared_memory( void *obj ) {
	if( shmdt(obj)==-1 ) return -1;

	return 0;
}
