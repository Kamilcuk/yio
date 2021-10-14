#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <sys/prctl.h> // prctl(), PR_SET_PDEATHSIG

/* -------------------------------------------------------------------------------- */

extern const char *__progname;

static inline
void _erron(int status, int is_fatal, const char *func, int linenum, const char *fmt, ...) {
	if (!status) return;
	fflush(stdout);
	va_list va;
	va_start(va, fmt);
	fprintf(stderr, "%s:%s:%d: ", __progname, func, linenum);
	vfprintf(stderr, fmt, va);
	if (errno) {
		fprintf(stderr, ": %s", strerror(errno));
	}
	fprintf(stderr, "\n");
	fflush(stderr);
	if (is_fatal) {
		exit(EXIT_FAILURE);
	}
	va_end(va);
}

#define errmsg(...)               _erron(1, 1, __func__, __LINE__, __VA_ARGS__)
#define erron(expr)               _erron(!!(expr), 1, __func__, __LINE__, "Expression %s failed", #expr)
#define erronmsg(expr, fmt, ...)  _erron(!!(expr), 1, __func__, __LINE__, "Expression %s failed: " fmt, #expr, ##__VA_ARGS__) // NOLINT
#define erron_noexit(expr)        _erron(!!(expr), 0, __func__, __LINE__, "Expression %s failed", #expr)

/* -------------------------------------------------------------------------------- */

static void usage() {
	printf(
		"Usage: redirect_stdin string prog...\n"
		"\n"
		"Redirect the first argument beeing a string of any character\n"
		"as standard input to program.\n"
		"\n"
		"Written by Kamil Cukrowski 2020\n"
		"\n"
	);
	exit(EXIT_SUCCESS);
}

static int childprocess(int fd[2], char *cmd[]) {
	erron(close(fd[1]) == -1);
	erron(dup2(fd[0], STDIN_FILENO) == -1);
	erronmsg(execvp(cmd[0], cmd), "cmd[0]=%s", cmd[0]);
	return EXIT_FAILURE;
}

static pid_t child = -1;

static void atexitkillchild(void) {
	if (child != -1) {
		erron_noexit(kill(child, SIGHUP) == -1);
	}
}

static int parentprocess(int fd[2], const char *str) {
	erron(close(fd[0]) == -1);
	size_t length = strlen(str);
	errno = 0;
	while (length) {
		const ssize_t writeret = write(fd[1], str, length);
		if (writeret == -1) {
			if (errno == EPIPE) {
				break;
			}
			if (errno == EAGAIN) {
				continue;
			}
		}
		erronmsg(writeret == -1, "write to child failed");
		str += writeret;
		length -= writeret;
	}
	// add a newline
	if (errno != EPIPE) {
		const ssize_t writeret = write(fd[1], "\n", 1);
		erronmsg(writeret == -1, "write to child failed");
	}

	erron(close(fd[1]) == -1);
	int stat = 0;
	erron(waitpid(child, &stat, 0) == -1);
	child = -1;
	if (WIFEXITED(stat)) {
		return WEXITSTATUS(stat);
	} else if (WIFSIGNALED(stat)) {
		errmsg("child received a %d signal: %s", WTERMSIG(stat), strsignal(WTERMSIG(stat)));
		return 128 + WTERMSIG(stat);
	} else {
		errmsg("child did not exit normally nor received a signal");
	}
	return -1;
}

int main(int argc, char *argv[]) {
	erron(prctl(PR_SET_PDEATHSIG, SIGHUP) == -1);
	erron(atexit(atexitkillchild) == -1);

	if (argc <= 1) {
		usage();
	}
	erronmsg(argc < 3, "Not enough arguments %d", argc);
	int fd[2] = {0};
	erron(pipe(fd) == -1);
	pid_t forkret = 0;
	erron((forkret = fork()) == -1);
	if (forkret == 0) {
		return childprocess(fd, &argv[2]);
	}

	child = forkret;
	return parentprocess(fd, argv[1]);
}
