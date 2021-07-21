#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <errno.h>

struct thread_info {
          pthread_t thread_id;
          int       thread_num;
      };

#define NUM_THREADS 50

int a=0;

static void *tfunc(void *args) {
	for (int i=0; i<1000; i++) {
		a++;
		printf("a = %d\n", a);
	}
	return 0;
}


int main() {
	struct thread_info tinfo[NUM_THREADS];
	int s, tnum;
	void *res;

	for(tnum=0; tnum<NUM_THREADS; tnum++) {
		tinfo[tnum].thread_num = tnum + 1;
		pthread_create(&tinfo[tnum].thread_id, NULL, tfunc, &tinfo[tnum]);
	}

	for (tnum = 0; tnum < NUM_THREADS; tnum++) {
		s = pthread_join(tinfo[tnum].thread_id, (void**) &res);
		//sleep(1);
		//printf("errno = %d %d %d\n", errno, s, tnum);
		errno=0;
	}
	
	return 0;
}