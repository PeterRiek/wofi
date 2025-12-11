/*
 *  Copyright (C) 2019-2020 Scoopta
 *  This file is part of Wofi
 *  Wofi is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Wofi is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Wofi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <utils.h>

#include <libgen.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>

time_t utils_get_time_millis(void) {
	struct timeval time;
	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}

void utils_sleep_millis(time_t millis) {
	struct timespec time;
	time.tv_sec = millis / 1000;
	time.tv_nsec = (millis % 1000) * pow(1000, 2);
	nanosleep(&time, NULL);
}

char* utils_concat(size_t arg_count, ...) {
	va_list args;
	va_start(args, arg_count);
	size_t buf_s = 1;
	for(size_t count = 0; count < arg_count; ++count) {
		buf_s += strlen(va_arg(args, char*));
	}
	va_end(args);
	va_start(args, arg_count);
	char* buffer = malloc(buf_s);
	strcpy(buffer, va_arg(args, char*));
	for(size_t count = 0; count < arg_count - 1; ++count) {
		strcat(buffer, va_arg(args, char*));
	}
	va_end(args);
	return buffer;
}

size_t utils_min(size_t n1, size_t n2) {
	return n1 < n2 ? n1 : n2;
}

size_t utils_max(size_t n1, size_t n2) {
	return n1 > n2 ? n1 : n2;
}

size_t utils_min3(size_t n1, size_t n2, size_t n3) {
	if(n1 < n2 && n1 < n3) {
		return n1;
	} else if(n2 < n1 && n2 < n3) {
		return n2;
	} else {
		return n3;
	}
}

size_t utils_distance(const char* haystack, const char* needle) {
	size_t str1_len = strlen(haystack);
	size_t str2_len = strlen(needle);

	size_t arr[str1_len + 1][str2_len + 1];
	arr[0][0] = 0;
	for(size_t count = 1; count <= str1_len; ++count) {
		arr[count][0] = count;
	}
	for(size_t count = 1; count <= str2_len; ++count) {
		arr[0][count] = count;
	}

	uint8_t cost;
	for(size_t c1 = 1; c1 <= str1_len; ++c1) {
		for(size_t c2 = 1; c2 <= str2_len; ++c2) {
			if(haystack[c1 - 1] == needle[c2 - 1]) {
				cost = 0;
			} else {
				cost = 1;
			}
			arr[c1][c2] = utils_min3(arr[c1 - 1][c2] + 1, arr[c1][c2 - 1] + 1, arr[c1 - 1][c2 - 1] + cost);
		}
	}

	if(strstr(haystack, needle) != NULL) {
		arr[str1_len][str2_len] -= str2_len;
	}

	return arr[str1_len][str2_len];
}

void utils_mkdir(char* path, mode_t mode) {
	if(access(path, F_OK) != 0) {
		char* tmp = strdup(path);
		utils_mkdir(dirname(tmp), mode);
		mkdir(path, mode);
		free(tmp);
	}
}

int utils_run_command(const char *cmd, char *const params[], char **output)
{
    int pipefd[2];
    pid_t pid;
    int status;

    *output = NULL;

    if (pipe(pipefd) == -1)
        return -1;

    pid = fork();
    if (pid < 0) {
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    }

    if (pid == 0) {
        // Child: redirect stdout -> pipe
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        execvp(cmd, params);   // PATH lookup + params
        _exit(127);            // exec failed
    }

    // Parent: read output
    close(pipefd[1]);

    size_t size = 0;
    size_t used = 0;
    char *buf = NULL;

    char tmp[256];
    ssize_t n;

    while ((n = read(pipefd[0], tmp, sizeof(tmp))) > 0) {
        if (used + n + 1 > size) {
            size_t new_size = size == 0 ? 512 : size * 2;
            if (new_size < used + n + 1)
                new_size = used + n + 1;

            char *new_buf = realloc(buf, new_size);
            if (!new_buf) {
                free(buf);
                close(pipefd[0]);
                waitpid(pid, NULL, 0);
                return -1;
            }
            buf = new_buf;
            size = new_size;
        }
        memcpy(buf + used, tmp, n);
        used += n;
    }

    close(pipefd[0]);

    if (buf)
        buf[used] = '\0';

    *output = buf;

    waitpid(pid, &status, 0);

    if (WIFEXITED(status))
        return WEXITSTATUS(status);

    return -1;
}
