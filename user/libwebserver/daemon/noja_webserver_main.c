/** @file noja_webserver_main.c
 * REL-20 Webserver daemon implementation
 *
 * @version "$"
 */
/*
 * LIMITATIONS
 *
 * This document is copyright © NOJA Power Switchgear Pty Ltd 2020. It contains
 * confidential intellectual property that belongs to NOJA Power Switchgear Pty
 * Ltd. It does NOT invest any rights to that intellectual property in the
 * recipient.
 *
 * This document is provided solely for limited use by the recipient, who is
 * NOT permitted in any way to copy, or to disclose to any other party, any
 * part of the contents of this document directly or indirectly without the
 * express written permission of NOJA Power Switchgear Pty Ltd.
 */
#include <stdio.h>
#if !defined(_MSC_VER)
#include <unistd.h>
#endif
#include <stdlib.h>
#include <limits.h>
#include <signal.h>
#include "noja_webserver_main.h"
#include "noja_webserver.h"
#include "noja_webserver_cfg.h"
#include "datapointEnum.h"
#if !defined(QT_CORE_LIB)
    #include "dbApi.h"
    #include "libsmp.h"
#endif

/** Flag signifying the process needs to be terminated soon
  * 0: Not terminating, otherwise terminating */
volatile sig_atomic_t terminating = 0;

#if !defined(QT_CORE_LIB)
/** Handler when signal is raised by the OS.
  * Set the termination flag if SIGINT or SIGTERM is received
  *
  * @param[in] signo Signal that occured
  */
static void nwsDaemonTerminate(int signo)
{
    if(    signo == SIGTERM
        || signo == SIGINT) {
        terminating = 1;
    }
}
#endif

static void display(const char *name, const char* msg)
{
    fprintf(stdout, "NOJA REL-20 Webserver v%d.%d\n", NWS_VERSION_MAJOR, NWS_VERSION_MINOR);
    if(msg) {
        fprintf(stderr, "Error: %s\n", msg);
    }
    fprintf(stdout, "Usage: %s [-p port] [-s service interval] [-i ping-pong interval] -h\n", name);
    fprintf(stdout, "Options:\n");
    fprintf(stdout, "        -p\n");
    fprintf(stdout, "              Specify the communication port to use when establishing HTTP and Websocket connection.\n");
    fprintf(stdout, "              Normally set to 80, 443(default) or 8080. Valid values are 0-65535 \n");
    fprintf(stdout, "        -s\n");
    fprintf(stdout, "              Specify the webserver process service interval. This is the minimum amount of time\n");
    fprintf(stdout, "              (in millisecond) the Webserver process will be blocked to allow the processes with\n");
    fprintf(stdout, "              equal or lower priority CPU time.Valid value range is 0-65535 ms\n");
    fprintf(stdout, "        -i\n");
    fprintf(stdout, "              Specify the amount of time (in seconds) the Webserver will wait for the pong reply to\n");
    fprintf(stdout, "              its ping request before terminating the connection. \n");
    fprintf(stdout, "        -h\n");
    fprintf(stdout, "              Display this help.\n");
    fprintf(stdout, "Examples:\n");
    fprintf(stdout, "        %s\n", name);
    fprintf(stdout, "        %s -p 443 -s 100 -i 10\n", name);
    fprintf(stdout, "        %s -p 443\n", name);
    fprintf(stdout, "        %s -s 100 -i 10\n", name);
    fprintf(stdout, "        %s -h\n", name);
    fprintf(stdout, "        %s\n", name);
}

int main(int argc, char **argv)
{
    NwsHttpPort port = NWS_PORT_DEFAULT;
    NwsTimeIntervalMS serviceInterval = NWS_SERVICE_INTERVAL_DEFAULT;
    NwsTimeIntervalS pingPongInterval = NWS_PINGPONG_INTERVAL_DEFAULT;
#if !defined(QT_CORE_LIB)
    int opt, parameter, ret;
    struct sigaction sigAction;
#else
#if !defined(_MSC_VER)
    int opt, parameter;
#endif
    char indicators[] = {'\\', '|', '/', '-'};
    unsigned long idx = 0;
    char buffer1[NWS_MAX_PATH_LENGTH];
    char buffer2[NWS_MAX_PATH_LENGTH];
#endif
    NwsContext context = (NwsContext)0;
    NwsErrors nwsError;

#if !defined(_MSC_VER)
    while ((opt = getopt(argc, argv, "p:s:i:h")) != -1) {
        switch (opt) {
            case 'p':
                parameter = atoi(optarg);
                if(    parameter < 0
                    || parameter > USHRT_MAX) {
                    display(argv[0], "The specified port is out of range");
                    return -1;
                }
                port = (NwsHttpPort)parameter;
                break;
            case 's':
                parameter = atoi(optarg);
                if(    parameter < 0
                    || parameter > USHRT_MAX) {
                    display(argv[0], "The specified service interval is out of range");
                    return -1;
                }
                serviceInterval = (NwsTimeIntervalMS)parameter;
                break;
            case 'i':
                parameter = atoi(optarg);
                if(    parameter < 0
                    || parameter > UCHAR_MAX) {
                    display(argv[0], "The specified ping-pong interval is out of range");
                    return -1;
                }
                pingPongInterval = (NwsTimeIntervalS)parameter;
                break;
            case 'h':
                display(argv[0], NULL);
                return 0;
            default:
                break;
       }
    }
#endif
    terminating = 0;
#if !defined(QT_CORE_LIB)
    // config the sigaction structure for our purposes
    sigemptyset(&sigAction.sa_mask);
    sigAction.sa_flags = 0;
    sigAction.sa_handler = nwsDaemonTerminate;

    // finally, activate the signal handlers!
    if (sigaction(SIGINT, &sigAction, NULL) < 0) {
        SYSERR(-ERR_WEBSERVER, "Failed to register SIGINT handle!");
        return -ERR_WEBSERVER;
    }
    if (sigaction(SIGTERM, &sigAction, NULL) < 0) {
        SYSERR(-ERR_WEBSERVER, "Failed to register SIGTERM handle!");
        return -ERR_WEBSERVER;
    }

#if defined(HOST)
    if (system("/bin/pidof dbserver > /dev/null") != 0) {
        if(system("~/Code/rc20-user/user/dbserver/startDbserverHost.sh -p > /dev/null") != 0) {
            fprintf(stderr, "Error: Failed to start dbserver!\n");
            return -1;
        }
    }
#endif

    /* Init the client accessor routines */
    if ((ret = dbClientInit(DbClientId_Webserver, 0, 0, argv[0])) != DB_SUCCESS) {
        SYSERR(-ERR_WEBSERVER, "Failed to initialise dbapi client accessor! Error code: %d", ret);
        return -ERR_WEBSERVER;
    }

    /* smp tick is 10x the service interval */
    if ((ret = smp_init("Webserver", DbClientId_UserCredential, serviceInterval * 10)) != 0)  {
#if !defined(NWS_IGNORE_SMP_ERROR)
        dbClientShutdown();
        SYSERR(-ERR_WEBSERVER, "Failed to initialise SMP! Error code: %d", ret);
        return -ERR_WEBSERVER;
#else
        SYSERR(SYS_INFO, "Failed to initialise SMP! Error code: %d", ret);
#endif
    }

    if ((ret = smp_ready()) != 0)  {
#if !defined(NWS_IGNORE_SMP_ERROR)
        dbClientShutdown();
        SYSERR(-ERR_WEBSERVER, "SMP not ready! Error code: %d", ret);
        return -ERR_WEBSERVER;
#else
        SYSERR(SYS_INFO, "SMP not ready! Error code: %d", ret);
#endif
    }
    if((nwsError = nwsInitialise(port, serviceInterval, pingPongInterval, NWS_MOUNT_DIR, &context)) != NwsError_Ok) {
        dbClientShutdown();
#else
    getcwd(buffer1, sizeof(buffer1));
    snprintf(buffer2, sizeof(buffer2), "%s/mount", buffer1);

     if((nwsError = nwsInitialise(port, serviceInterval, pingPongInterval, buffer2, &context)) != NwsError_Ok) {
#endif
        SYSERR(-ERR_WEBSERVER, "Webserver library init failed! Error code: %d", nwsError);
        return -ERR_WEBSERVER;
    }

    SYSERR(SYS_INFO, "NOJA Webserver v%d.%d. Port: %d; ServiceInterval: %d; PingPongInterval: %d\n",
        NWS_VERSION_MAJOR, NWS_VERSION_MINOR, port, serviceInterval, pingPongInterval);
#if defined(QT_CORE_LIB)
    fprintf(stdout, "REL-20 Webserver Started!\n URL: http://<your ip>:%d\n", port);
    fprintf(stdout, "Activities: ");
#endif
    while (terminating == 0) {
#if defined(QT_CORE_LIB)
        fprintf(stdout, "%c\b", indicators[idx % 4]);
        fflush(stdout);
#endif
        if((nwsError = nwsProcess(context)) != NwsError_Ok) {
            nwsStop(&context);
#if !defined(QT_CORE_LIB)
            dbClientShutdown();
#endif
            SYSERR(-ERR_WEBSERVER, "Webserver library failed to service the request! Error code: %d", nwsError);
            return -ERR_WEBSERVER;
        }
#if !defined(QT_CORE_LIB)
        if ((ret = smp_tick()) != 0)  {
#if !defined(NWS_IGNORE_SMP_ERROR)
            nwsStop(&context);
            dbClientShutdown();
            SYSERR(-ERR_WEBSERVER, "SMP Tick failed! Error code: %d", ret);
            return -ERR_WEBSERVER;
#else
            SYSERR(SYS_INFO, "SMP Tick failed! Error code: %d", ret);
#endif
        }
#else
        idx++;
#if !defined(_MSC_VER)
        usleep(10000);
#endif
#endif
    }

    if((nwsError = nwsStop(&context)) != NwsError_Ok) {
        SYSERR(SYS_INFO, "Failed to close the Webserver library!. Error code: %d", nwsError);
    }
#if !defined(QT_CORE_LIB)
    // release the DB resources
    dbClientShutdown();
#endif

    SYSERR(SYS_INFO, "NOJA REL-20 Webserver Terminated!.");


    return 0;
}
