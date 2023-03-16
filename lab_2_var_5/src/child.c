#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

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

int is_prime_or_negative(const long long n) {
    if (n < 0 || n == 2) return 1;
    if (!(n&1) || n == 1) return 0;
    for (long long i = 3; i * i <= n; i+=2) {
        if (n%i == 0) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, const char *argv[]) {
    // check that we have all necessary arguments
    if (argc < 4) {
        send_error_and_stop("Arguments missing\n", 1);
    }
    // fd to read and write from argv[2] and argv[3]
    int fd[2];
    // transform strings to int
    fd[READ_END] = atoi(argv[2]);
    fd[WRITE_END] = atoi(argv[3]);

    // open file
    int fp = open(argv[1], O_CREAT|O_WRONLY, 0777);
    if (fp == -1) {
        send_error_and_stop("Cannot open file\n", 1);
    }

    long long N;    // user input from parent process
    short ans = 1;  // ans to parent process (0 = stop)
    while(ans == 1) {
        char buff [18];
        int len = 0;
        // max len for long long
        if (read(fd[READ_END], &buff, 18) == -1) {
            send_error_and_stop("Child Cannot read from fd\n", 1);
        }
        for (int i = 0; i < 18; ++i) {
            if (buff[i] == '\n') {
                for (int j = i + 1; j < 18; ++j) {
                    buff[j] = '\0';
                }
                len = i + 1;
                break;
            }
        }
        // convert str to long long int
        N = atoll(buff);

        // update ans
        ans = 1 - is_prime_or_negative(N);

        if (ans == 1) {
            // print N to file
            write(fp, &buff, sizeof(char) * len);
        }

        if (write(fd[WRITE_END], &ans, sizeof(short)) == -1) {
            send_error_and_stop("Cannot write to fd\n", 1);
        }
    }
    if (close(fd[WRITE_END]) == -1 || close(fd[READ_END]) == -1 || close(fp) == -1) {
        send_error_and_stop("Cannot close fd\n", 1);
    }
    return 0;
}