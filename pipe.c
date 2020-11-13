#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>

int main(){
struct sembuf mybuf;
int fd[2], result, semid;
size_t size;
char resstring[14], pathname[]="08-2a.c";
key_t key;

/* Попытаемся создать pipe */

if(pipe(fd) < 0){

/* Если создать pipe не удалось, печатаем об этом сообщение и прекращаем работу */

printf("Can\'t create pipe\n");
exit(-1);
}

/* Порождаем новый процесс */

result = fork();

if(result <0){

/* Если создать процесс не удалось, сообщаем об этом и завершаем работу */

printf("Can\'t fork child\n");
exit(-1);

} else if (result > 0) {

/* Мы находимся в родительском процессе, который будет передавать информацию процессу-ребенку */
/* Пробуем записать в pipe 14 байт, т.е. всю строку "Hello, world!"
вместе с признаком конца строки */
printf("parent start\n");
printf("parent write: Hello, world!\n");
size = write(fd[1], "Hello, world!", 14);

if(size != 14){

/* Если записалось меньшее количество байт, сообщаем об ошибке и завершаем работу */

printf("Can\'t write all string\n");
exit(-1);
}

/* Закрываем входной поток данных и на этом родитель прекращает работу */

close(fd[1]);

if((key = ftok(pathname,0)) < 0){
printf("Can\'t generate key\n");
exit(-1);
}

/* Пытаемся получить доступ по ключу к массиву семафоров, если он существует,
или создать его из одного семафора, если он еще не существует, с правами доступа
read & write для всех пользователей */

if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0){
printf("Can\'t get semid\n");
exit(-1);
}

mybuf.sem_op = -1;
mybuf.sem_flg = 0;
mybuf.sem_num = 0;
sleep(1);
if(semop(semid, &mybuf, 1) < 0){
printf("Can\'t wait for condition\n");
exit(-1);
}

size = read(fd[0], resstring, 3);
printf("Parent read: %s\n", resstring);
if(size < 0){

/* Если прочитать не смогли, сообщаем об ошибке и завершаем работу */

printf("Can\'t read string\n");
exit(-1);
}

/* Печатаем прочитанную строку */


close(fd[0]);

sleep(1);

printf("Parent exit\n");

} else {

/* Мы находимся в порожденном процессе, который будет получать информацию от процесса-родителя. Он унаследовал от родителя таблицу открытых файлов и, зная файловые дескрипторы, соответствующие pip'у, может его использовать. В этом процессе входной поток данных нам не понадобится, поэтому закрываем его.*/

printf("child start\n");

/* Пробуем прочитать из pip'а 14 байт в массив, т.е. всю записанную строку */

size = read(fd[0], resstring, 14);

if(size < 0){

/* Если прочитать не смогли, сообщаем об ошибке и завершаем работу */

printf("Can\'t read string\n");
exit(-1);
}
close(fd[0]);
/* Печатаем прочитанную строку */

printf("Child read: %s\n",resstring);
printf("Child write: Ok\n");
size = write(fd[1], "Ok", 3);

if(size != 3){

/* Если записалось меньшее количество байт, сообщаем об ошибке и завершаем работу */

printf("Can\'t write all string\n");
exit(-1);
}

if((key = ftok(pathname,0)) < 0){
printf("Can\'t generate key\n");
exit(-1);
}

/* Пытаемся получить доступ по ключу к массиву семафоров, если он существует,
или создать его из одного семафора, если он еще не существует, с правами доступа
read & write для всех пользователей */

if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0){
printf("Can\'t get semid\n");
exit(-1);
}

mybuf.sem_op = 1;
mybuf.sem_flg = 0;
mybuf.sem_num = 0;

if(semop(semid, &mybuf, 1) < 0){
printf("Can\'t wait for condition\n");
exit(-1);
}

/* Закрываем входной поток и завершаем работу */
close(fd[1]);
printf("child exit\n");
}

return 0;
}
