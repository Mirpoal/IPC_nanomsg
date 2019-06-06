//
// Created by EShare on 2019/6/5.
//

#include "device_demo.h"

using namespace felix;

#define url_server "tcp://127.0.0.1:5554"


int main() {

    device_demo::server(url_server);

    return 0;
}