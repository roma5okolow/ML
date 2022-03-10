#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main() {
    char *bash_argv = {"sh", NULL};
    char *buf[2 << 20];
    int master;
    int ret;

    master = posix_openpt(O_RDWR | O_NOCTTY);
    if(master < 0)
    {
        perror("opent");
        return 1;
    }

    if (grantpt(master)){
        perror("grantpt");
        return 1;
    }

    if(unlockpt(master)){
        perror("unclockpt");
        return 1;
    }

    ret = fork();
    if (ret == 0){
        int term;

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        term = open(ptsname(master), O_RDWR);
        if (term < 0){
            perror("open slave term");
            exit(1)
        }

        dup2(term, STDIN_FILENO);
        dup2(term, STDOUT_FILENO);
        dup2(term, STDERR_FILENO);

        close(master);

        execvp("bash", bash_argv);
    }

#define LS "ls -la /proc/self/fd"
        ret = write(master, LS, sizeof(LS));
        if(ret != sizeof(LS)){
            perror("write");
            return 1;
        }

        ret = read(master, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, ret);

        write(master, "exit", sizeof("exit"));

        close(master);
        return 0;
}