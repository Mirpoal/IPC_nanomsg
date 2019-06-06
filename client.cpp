//
// Created by EShare on 2019/6/5.
//
#include "device_demo.h"

using namespace felix;

#define url_client "tcp://127.0.0.1:5555"

int main() {

    device_demo::client(url_client, "felix");

    return 0;
}