/*
1. fork off the parent process & let it terminate if forking was successful. -> Because the parent process has terminated, the child process now runs in the background.
2. setsid - Create a new session. The calling process becomes the leader of the new session and the process group leader of the new process group. The process is now detached from its controlling terminal (CTTY).
3. Catch signals - Ignore and/or handle signals.
4. fork again & let the parent process terminate to ensure that you get rid of the session leading process. (Only session leaders may get a TTY again.)
5. chdir - Change the working directory of the daemon.
6. umask - Change the file mode mask according to the needs of the daemon.
7. close - Close all open file descriptors that may be inherited from the parent process.

Please note this program has been referred from a stackoverflow answer and implemented with minor changes
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

static void my_daemon()
{
    pid_t pid;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    /* Catch, ignore and handle signals */
    //TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /* Fork off for the second time*/
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* Set new file permissions */
    umask(0);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir("/");

    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>0; x--)
    {
        close (x);
    }

    /* Open the log file */
    openlog ("mydaemon", LOG_PID, LOG_DAEMON);
}


int main()
{
    my_daemon();

    while (1)
    {
        //daemon exits after 1 minute
        syslog (LOG_NOTICE, "Daemon started.");
        sleep (60);
        break;
    }

    syslog (LOG_NOTICE, "Daemon terminated.");
    closelog();

    return EXIT_SUCCESS;
}
