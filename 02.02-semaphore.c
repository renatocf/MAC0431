#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>

/* Useful definitions */
typedef enum { false, true } bool;
#define UNUSED(x) (void)(x)
#define EXPRESSION(expr) do { expr; } while (false);

union semun {
    int val;                    /* value for SETVAL */
    struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
    unsigned short int *array;  /* array for GETALL, SETALL */
    struct seminfo *__buf;      /* buffer for IPC_INFO */
};

typedef int * SHARED;

SHARED x;

struct sembuf smb;
union semun arg;

#define SEM_CALL   EXPRESSION(semop(semid, &smb, 1))
#define SEM_SET(x) EXPRESSION(arg.val = x; semctl(semid, 0, SETVAL, arg))
#define SEM_P()    EXPRESSION(smb.sem_op = -1; SEM_CALL)
#define SEM_V()    EXPRESSION(smb.sem_op =  1; SEM_CALL)

int main(int argc, char **argv)
{
    int i, j;
    int smid, semid;

    int sim = argc > 1;
    UNUSED(argv);

    smid = shmget(42, sizeof(int), IPC_CREAT | 0600);
    x = (SHARED) shmat(smid, 0, 0);

    semid = semget(42, 1, IPC_CREAT | 0600);
    smb.sem_num = 0;
    smb.sem_flg = 0;

    SEM_SET(1);

    *x = 0;

    if (fork() == 0) {
        int y;

        for (i = 0; i < 5; i++) {
            puts("Filho");
            if (sim) {
                SEM_P();
            }
            for (j = 0; j < 100000; j++) {
                y = *x;
                y += 1;
                *x = y;
            }
            SEM_V();
        }
    } else {
        int y;

        for (i = 0; i < 5; i++) {
            puts("Filho");
            if (sim) {
                SEM_P();
            }
            for (j = 0; j < 100000; j++) {
                y = *x;
                y += 1;
                *x = y;
            }
            SEM_V();
        }

        wait(NULL);
        printf("x = %d\n", *x);
    }

    return 0;
}

