// To implement Producer-consumer using shared memory
//Implement the above using shm_open and mmap

//PRODUCER PROGRAM SHARED MEMORY
 /*programs to illustrate shared memory implementation using the following system calls: shm_open, ftruncate(), mmap, shm_unlink*/
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/mman.h>

int main() 
{
	void * psm;
	char buf[10];
	int shmid1;
	shmid1 = shm_open("OS", O_CREAT | O_RDWR, 0666);
	ftruncate(shmid1, 1024);
	psm = mmap(0, 1024, PROT_WRITE, MAP_SHARED, shmid1, 0);
	printf("Key/ID/fd of the shared memory: %d\n", shmid1);
	printf("shared memory is attached at:%p\n", psm);
	printf("enter some data to write to shared memory\n");
	read(0, buf, 10);
	sprintf(psm, "%s", buf);
	return 0;
}

// CONSUMER PROGRAM SHARED MEMORY
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
int main() 
{
	void *psm;
	int shmid;
	shmid = shm_open("OS", O_RDONLY, 0666);
	ftruncate(shmid, 1024);
	psm = mmap(0, 1024, PROT_READ, MAP_SHARED, shmid, 0);
	printf("Key/ID/fs of the shared memory: %d\n", shmid);
	printf("shared memory for consumer is attached at:%p\n", psm);
	printf("%s", (char *)psm);
	printf("\n");
	shm_unlink("OS");//removes the name of the shared memory object named by the string pointed to by name.
	return 0;
}



//Implement the above using shmget and shmat
//USING SHMGET, SHMAT, SHMDT, SHMCTL SYSTEM CALLS
//PRODUCER PROCESS
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
int main() 
{
	void * psm;
	char buf[10];
	int shmid;
	//To get the shared memory id
	shmid = shmget((key_t) 1122, 1024, 0666 | IPC_CREAT);
	printf("Key of the shared memory: %d\n", shmid);
	//To attach shared memory segment to an address space
	psm = shmat(shmid, NULL, 0666);
	printf("memory attached at:%p\n", psm);
	printf("enter some data to write to shared memory\n");
	read(0, buf, 10);
	sprintf(psm, "%s", buf);
	//To detach the shared memory segment from the address space of the calling process.
	shmdt(psm);
	return 0;
}

//CONSUMER PROCESS
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<string.h>
int main()
{
	void * psm;
	char buf[10];
	int shmid;
	shmid = shmget((key_t) 1122, 1024, 0666);
	printf("Key of the shared memory: %d\n", shmid);
	psm = shmat(shmid, NULL, 0666);
	printf("process attached at:%p\n", psm);
	printf("data read from the shared memory is:%s", (char * ) psm);
	printf("\n");
	//shmctl() performs the control operation specified by cmd
	shmctl(shmid, IPC_RMID, NULL); //IPC_RMID marks the segment to destroyed
	return 0;
}


Write a program to generate and print Fibonacci series with the following
requirements:
- Parent program should create a child and distribute the task of generating
  Fibonacci no to its child.
- The code for generating Fibonacci no. should reside in different program.
- Child should write the generated Fibonacci sequence to a shared memory.
- Parent process has to print by retrieving the Fibonacci sequence from the shared memory.
a) Implement the above using shm_open and mmap
b) Implement the above using shmget and shmat
Note: Shared object should be removed at the end in the program.

//using shm_open and mmap
//parent.c
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
int main(int argc,char *argv[])
{
    int i;
    pid_t pid;
    int k;
    int n1,n2,n3;
    const int SIZE = 4096;
        int shm_fd;
    void *ptr;
    if (argc > 1)
    {
        sscanf(argv[1], "%d", &i);
        if (i < 1)
        {
            printf("Error input: %d\n", i);
            return 0;
        }
    }
    else
    {
      printf("Error: not passing N in command line\n");
      exit(0);
    }
    pid = fork();
    if (pid == 0)// CHILD
    {   
        execlp("./fib","fib",argv[1],NULL);
    }
    else if (pid > 0)
    {
        wait(NULL);
        printf("\nPARENT: child completed\n");

        shm_fd = shm_open("OS", O_RDONLY, 0666);
        ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
        printf("Parent printing:\n");
        printf("%s ", (char *)ptr);
        shm_unlink("OS");
    }
    return 0;
}

//child.c

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
         int k=2,n1,n2,n3;
         void*ptr;
         int shm_fd = shm_open("OS", O_CREAT | O_RDWR, 0666);
         ftruncate(shm_fd,4096);
         ptr = mmap(0,4096,PROT_WRITE, MAP_SHARED, shm_fd, 0);
         printf("CHILD:\n");
         int i=atoi(argv[1]);
         n1=0;
         n2=1;
         sprintf(ptr,"%d ",n1);
         ptr+=strlen(ptr);
         printf("%d ",n1);
         sprintf(ptr,"%d ",n2);
         ptr+=strlen(ptr);
         printf("%d ",n2);
         while (k!=i)
         {
                n3=n1+n2;
                sprintf(ptr,"%d ", n3);
                printf("%d ", n3);
                n1=n2;
                n2=n3;
                ptr += strlen(ptr);
                k++;
	}
}
	

Output:
$ gcc parent.c -lrt
$ gcc child.c -o fib -lrt
$ ./a.out 5
CHILD:
0 1 1 2 3
PARENT: child completed
Parent printing:
0 1 1 2 3


ii) Using shmget
parent.c:
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
int main(int argc, char *argv[])
{
	int i;
	pid_t pid;
	int k;
	int n1, n2, n3;
	const int SIZE = 4096;
	int shmid;
	void *ptr;
	shmid = shmget((key_t) 1122, 4096, 0666 | IPC_CREAT);
	ptr = shmat(shmid, NULL, 0666);
	if (argc > 1)
	{
	sscanf(argv[1], "%d", &i);
	if (i < 1)
	{
	printf("Error input: %d\n", i);
	return 0;
	}
	}
	else
	{
	return 1;
	}
	pid = fork();
	if (pid == 0)
	{
	execlp("./fib", "fib", argv[1], NULL);
	}
	else if (pid > 0)
	{
	wait(NULL);
	printf("\nPARENT: child completed\n");
	printf("Parent printing:\n");
	printf("%s ", (char *)ptr);
	shmdt(ptr);
	
	}
	return 0;
}

child.c:
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
	int k = 2, n1, n2, n3;
	void *ptr;
	int shmid = shmget((key_t) 1122, 4096, 0666);
	ptr = shmat(shmid, NULL, 0666);
	printf("CHILD:\n");
	int i = atoi(argv[1]);
	n1 = 0;
	n2 = 1;
	sprintf(ptr, "%d ", n1);
	ptr += strlen(ptr);
	printf("%d ", n1);
	sprintf(ptr, "%d ", n2);
	ptr += strlen(ptr);
	printf("%d ", n2);
	while (k != i)
	{
	n3 = n1 + n2;
	sprintf(ptr, "%d ", n3);
	printf("%d ", n3);
	n1 = n2;
	n2 = n3;
	ptr += strlen(ptr);
	k++;
	}
	shmctl(shmid, IPC_RMID, NULL);
	return 0;
}
Output:
$ gcc parent.c
$ gcc child.c -o fib
$ ./a.out 5
CHILD:
0 1 1 2 3
PARENT: child completed
Parent printing:
0 1 1 2 3


