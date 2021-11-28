#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

char shm_fn[] = "shm";
char sem_fn[] = "sem";
static int counter = 1;

main()
{
    caddr_t shmptr;
    unsigned int mode;
    int shmdes, index;
    sem_t *semdes;
    int SHM_SIZE;

    srand(time(NULL));

    mode = S_IRWXU | S_IRWXG;

    //Open the shared memory object

    if ((shmdes = shm_open(shm_fn, O_CREAT | O_RDWR | O_TRUNC, mode)) == -1)
    {
        perror("shm_open failure");
        exit();
    }

    // Preallocate a shared memory area

    SHM_SIZE = sysconf(_SC_PAGE_SIZE);

    if (ftruncate(shmdes, SHM_SIZE) == -1)
    {
        perror("ftruncate failure");
        exit();
    }

    if ((shmptr = mmap(0, SHM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED,
                       shmdes, 0)) == (caddr_t)-1)
    {
        perror("mmap failure");
        exit();
    }

    //Create a semaphore in locked state

    sem_des = sem_open(sem_fn, O_CREAT, 0644, 0);

    if (sem_des == (void *)-1)
    {
        perror("sem_open failure");
        exit();
    }

    // Access to the shared memory area

    while (counter != 1000)
    {

        int random_number = (rand() % 2) + 1;
        if (random_number == 2)
        {
            printf("write %d into the shared memory shmptr[%d]\n", counter, counter);
            shmptr[counter] = counter;
            counter++;
        }
    }
    // Release the semaphore lock

    sem_post(semdes);
    munmap(shmptr, SHM_SIZE);

    // Close the shared memory object

    close(shmdes);

    // Close the Semaphore

    sem_close(semdes);

    // Delete the shared memory object

    shm_unlink(shm_fn);
}