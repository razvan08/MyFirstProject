bool isPrime(int n)
{
    for (int i = 0; i <= n / 2; i++)
    {
        if (n % i == 0)
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    int fd[2];
    int lowerBound = 1;
    int upperBound = 1000;
    for (int process = 1; process <= 10; process++)
    {
        if (pipe(fd) == -1)
        {
            printf("Error in opening the pipe");
            return -1;
        }
        int id = fork();
        if (id == 0)
        {
            int inf = lowerBound;
            int sup = upperBound;
            close(fd[0]);
            write(fd[1], inf, sizeof(int));
            write(fd[1], sup, sizeof(int));
            lowerBound = upperBound + 1;
            upperBound += 1000;
            close(fd[1]);
        }
        else
        {
            close(fd[1]);
            int y;
            int z;
            read(fd[0], &y, sizeof(int));
            read(fd[0], &z, sizeof(int));
            for (int index = y; index <= z; index++)
            {
                if (isPrime(index) == true)
                {
                    printf("%d", index);
                }
            }
            close(fd[0]);
        }
    }
}