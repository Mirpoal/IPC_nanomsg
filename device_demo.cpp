//
// Created by EShare on 2019/6/5.
//

#include "device_demo.h"
#include <iostream>

namespace felix {
    int device_demo::server(const char *url) {
        int fd = nn_socket(AF_SP, NN_REP);
        if (fd < 0) {
            std::cerr << "nn_socket: " << nn_strerror(nn_errno()) << std::endl ;
            return -1;
        }

        if (nn_connect(fd, url) < 0) {
            std::cerr << "nn_connect: " << nn_strerror(nn_errno()) << std::endl ;
            nn_close(fd);
            return -1;
        }

        while (1) {
            char username[128];
            char greeting[128];
            time_t secs;
            struct tm *now;
            char *daytime;
            int rc;
            char *fmt;

            rc = nn_recv(fd, username, sizeof(username), 0);
            if (rc < 0) {
                std::cerr << "nn_recv: " << nn_strerror(nn_errno()) << std::endl ;
                break;
            }

            secs = time (NULL);
            now = localtime (&secs);
            if (now->tm_hour < 12) {
                daytime = "morning";

            } else if (now->tm_hour < 17) {
                daytime = "afternoon";

            } else if (now->tm_hour < 20) {
                daytime = "evening";

            } else {
                daytime = "night";
            }

            /*  Ensure ASCIIZ terminated string. */
            if (rc < sizeof (username)) {
                username[rc] = '\0';
            } else {
                username[sizeof (username) - 1] = '\0';
            }

            fmt = "Good %s, %s (from %d).";

            /*  Technically this might be overly pessimistic about size. */
            if ((strlen (username) + strlen (daytime) + strlen (fmt)) >=
                sizeof (greeting)) {

                fmt = "I'm sorry, your name is too long.  But good %s anyway.";
            }

            /*  snprintf would be safer, but the above check protects us. */
            sprintf (greeting, fmt, daytime, username, (int)getpid());

            rc = nn_send(fd, greeting, strlen(greeting), 0);
            if (rc < 0) {
                std::cerr << "nn_send: " << nn_strerror(nn_errno()) << "error" << std::endl ;
            }
        }
        nn_close(fd);
        return 0;
    }

    int device_demo::client(const char *url, const char *username) {
        int fd = nn_socket(AF_SP, NN_REQ);
        if (fd < 0) {
            std::cerr << "nn_socket: " << nn_strerror(nn_errno()) << std::endl ;
            return -1;
        }

        if (nn_connect(fd, url) < 0) {
            std::cerr << "nn_connect: " << nn_strerror(nn_errno()) << std::endl ;
            nn_close(fd);
            return -1;
        }

        usleep(1000);

        if (nn_send(fd, username, strlen(username), 0) < 0) {
            std::cerr << "nn_send: " << nn_strerror(nn_errno()) << std::endl ;
            nn_close(fd);
            return -1;
        }

        char* msg = NULL;
        int rc = nn_recv(fd, &msg, NN_MSG, 0);
        if (rc < 0) {
            std::cerr << "nn_recv: " << nn_strerror(nn_errno()) << std::endl ;
            nn_close(fd);
            return -1;
        }
        nn_close(fd);
        std::cout << msg << std::endl;
        nn_freemsg(msg);
        return 0;
    }


    int device_demo::device(const char *url1, const char *url2) {
        int s1 = nn_socket(AF_SP_RAW, NN_REQ);
        if (s1 < 0) {
            std::cerr << "nn_socket: " << nn_strerror(nn_errno()) << std::endl ;
            return -1;
        }

        if (nn_bind(s1, url1) < 0) {
            std::cerr << "nn_bind1: " << url1 << nn_strerror(nn_errno()) << std::endl ;
            return -1;
        }

        int s2 = nn_socket(AF_SP_RAW, NN_REQ);
        if (s1 < 0) {
            std::cerr << "nn_socket: " << nn_strerror(nn_errno()) << std::endl ;
            return -1;
        }

        if (nn_bind(s2, url2) < 0) {
            std::cerr << "nn_bind1: " << url2 << nn_strerror(nn_errno()) << std::endl ;
            return -1;
        }

        if (nn_device(s1, s2) != 0) {
            std::cerr << "nn_device: " << nn_strerror(nn_errno()) << std::endl ;
            return -1;
        }
        return 0;
    }

}// end of namespace felix
