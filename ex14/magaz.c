#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>

#define MAGNUM 5
#define NUM_THREADS 3
//static pthread_mutex_t foo_mutex[MAGNUM] = { P99_DUPL(MAGNUM, PTHREAD_MUTEX_INITIALIZER) }; // не работает
static pthread_mutex_t foo_mutex[MAGNUM] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};

struct thread_info {
	pthread_t thread_id;
	int       thread_num;
	int*		arg;
};

int a=0;

int action(int *args, int val) {
	int whilecount = 0, ret =0;
	while (val>0) {
		for (int i=0; i<MAGNUM; i++) {
			ret = pthread_mutex_trylock(&foo_mutex[i]);
			if (ret == 0) {
				val = val - args[i];
				args[i] = 0;
				printf(" I am %ld; mag[%d] = %d; need = %d\n", pthread_self(), i, args[i], val);
				pthread_mutex_unlock(&foo_mutex[i]);
				break;
			} else if (ret == EAGAIN || EBUSY){
				continue;
			} else {
				printf("error%d\n", i);
				return -1;
			}
		}
	printf("\n\n");
	sleep(2);
	}
	a++;
	return 0;
}

static void *tfunc(void *args) {
	int val = 0;
	srand(pthread_self());
	val = rand() % 1001 + 4500;
	args = (int*) args;
	printf("startneed%ld = %d\n", pthread_self(), val);
	if (action(args, val) != 0) {
		printf("error action\n");
	}
	return 0;
}

static void *tfunc2(void *args) {
	int val = 500, ret=0;
	int* argsi;
	argsi = args;
	while (a<NUM_THREADS) {
		for (int i=0; i<MAGNUM; i++) {
			ret = pthread_mutex_trylock(&foo_mutex[i]);
			if (ret == 0) {
				argsi[i] = argsi[i] + val;
				printf("\nIam pogruz %ld; mag[%d] = %d; added = %d\n\n", pthread_self(), i, argsi[i], val);
				pthread_mutex_unlock(&foo_mutex[i]);
			} else if (ret == EAGAIN || ret == EBUSY){
				continue;
			} else {
				printf("error%d\n", i);
				return 0;
			}
		}
		sleep(1);
	}
	return 0;
}


int main() {
	struct thread_info tinfo[NUM_THREADS];
	int s, tnum;
	int mag[MAGNUM] = {0, 0, 0, 0, 0};
	void *res;
	unsigned int seconds=0;
	srand(time(NULL));

	for(int i=0; i<MAGNUM; i++) {
		mag[i] = rand() % 101 + 950;
		printf("mag[%d] = %d\n", i, mag[i]);
	}

	for (int i = 0; i<NUM_THREADS; i++) {
		tinfo[i].arg = mag;
	}

	for(tnum=0; tnum<NUM_THREADS; tnum++) {
		tinfo[tnum].thread_num = tnum + 1;
		pthread_create(&tinfo[tnum].thread_id, NULL, tfunc, mag);
	}
	tinfo[tnum+1].thread_num = tnum + 1;
	pthread_create(&tinfo[tnum+1].thread_id, NULL, tfunc2, mag);
	for (tnum = 0; tnum < NUM_THREADS; tnum++) {
		s = pthread_join(tinfo[tnum].thread_id, &res);
	}
	s = pthread_join(tinfo[tnum+1].thread_id, &res);
	return 0;
}