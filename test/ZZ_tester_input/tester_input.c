#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>

static inline
void err(const char *str) {
	perror(str);
	kill(0, SIGTERM);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	if (argc < 3) err("Not enough arguments");
	int fd[2] = {0};
	const char *input = argv[1];
	if (pipe(fd)) err("pipe");
	switch (fork()){
	case -1:
		err("fork");
		break;
	case 0:
		// child
		close(fd[1]);
		if (dup2(fd[0], STDIN_FILENO)) err("dup2");
		close(fd[0]);
		execv(argv[2], &argv[2]);
		err("execv");
		break;
	}
	// parent
	close(fd[0]);
	if ((size_t)write(fd[1], input, strlen(input)) != strlen(input)) err("write");
	close(fd[1]);
	int stat;
	if (wait(&stat)) err("wait");
	return WIFEXITED(stat) ? WEXITSTATUS(stat) : -1;
}
