//
// Created by EShare on 2019/6/5.
//

#ifndef NANOMSG_DEVICE_DEMO_H
#define NANOMSG_DEVICE_DEMO_H

#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>

namespace felix {
    class device_demo {
    public:
        device_demo();
        ~device_demo();

        // server run forever
        static int server(const char* url);

        static int client(const char* url, const char* username);

        static int device(const char* url1, const char* url2);
    };
}  // end of namespace felix


#endif //NANOMSG_DEVICE_DEMO_H
