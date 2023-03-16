#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define WRITE_END 1
#define READ_END 0


void send_error_and_stop(char *massage, int code) {
    int i = 0;
    while (massage[i] != '\0') {
        i++;
    }
    write(STDERR_FILENO, massage, sizeof(char) * i);
    exit(code);
}

int power(int a, int b) {
    int res = 1;
    for (int i = 0; i < b; ++i) {
        res *= a;
    }
    return res;
}


int main(int argc, const char *argv[]) {
    char FileName[FILENAME_MAX];
    for (int i = 0; i < FILENAME_MAX; ++i) {
        FileName[i] = '\0';
    }
    int fd1[2];
    int fd2[2];
    int p1 = pipe(fd1);
    int p2 = pipe(fd2);
    // читаем название файла
    for (int i = 0; i < FILENAME_MAX; ++i) {
        if (read(STDIN_FILENO, &FileName[i], 1) < 1) {
            send_error_and_stop("Cannot read file name\n", -1);
        }
        // если конеч строки перестаём читать
        if (FileName[i] == '\n') {
            FileName[i] = '\0';
            break;
        }
    }
    if (p1 == -1 || p2 == -1) {
        send_error_and_stop("Pipe() error\n", 1);
    }
    int pid = fork();
    if (pid == -1) {
        send_error_and_stop("Fork() error\n", 1);
    } else if (pid == 0) {
// =================  Child  ================= //
        // close fd that we won't use
        if (close(fd1[WRITE_END]) == -1 || close(fd2[READ_END]) == -1) {
            send_error_and_stop("Cannot close fd\n", 1);
        }

        // transform int to str
        // is strings to store (int) fd so 12 characters would be enough
        char fd_read [12];
        char fd_write [12];
        // set last char to '\0' -- end of string
        fd_write[11] = 0;
        fd_read[11] = 0;
        for (int i = 10; i >= 0; --i) {
            fd_read[i] = (((fd1[READ_END] / power(10, (10 - i))) % 10) + '0');
            fd_write[i] = (((fd2[WRITE_END] / power(10, (10 - i))) % 10) + '0');
        }

        // args for child program
        char *Child_args[] = {"child", FileName, fd_read, fd_write, NULL};

        if (execv("child", Child_args) == -1) {
            send_error_and_stop("Cannot call exec child\n", 1);
        }
    } else {
// =================  parent ================= //
        // close fd that we won't use
        if (close(fd1[READ_END]) == -1 || close(fd2[WRITE_END]) == -1) {
            send_error_and_stop("Cannot close fd\n", 1);
        }

        short ans = 1;  // is answer from child: stop or not program
        while (ans == 1) {
            char buff [18];
            char c;
            int ind = 0;
            // max len for long long
            while (ind < 18) {
                read(STDIN_FILENO, &c, 1);
                buff[ind++] = c;
                if (c == '\n') {
                    break;
                }
            }

            if (write(fd1[WRITE_END], &buff, 18) == -1) {
                send_error_and_stop("Cannot write to fd\n", 1);
            }
            if (read(fd2[READ_END], &ans, sizeof(short)) == -1) {
                send_error_and_stop("Cannot read from fd\n", 1);
            }
        }
        // close all fd because we end to  write/read at all
        if (close(fd1[WRITE_END]) == -1 || close(fd2[READ_END]) == -1) {
            send_error_and_stop("Cannot close fd\n", 1);
        }

        // wait for child process end
        waitpid(pid, NULL, 0);
    }
    return 0;
}
