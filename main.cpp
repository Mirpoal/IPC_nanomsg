#include <iostream>
#include "device_demo.h"
#include "system_manager/system_manager.h"
#include "messenger/messenger.h"
#include "at_server/at_messenger.h"
#include "at_server/at_server.h"

using namespace eshare;

int main() {

    AtServerMessenger* server_messenger = new AtServerMessenger();

    SystemManager* g_systemManager = new SystemManager();

    g_systemManager->start();

    delete g_systemManager;
    g_systemManager = nullptr;

    delete server_messenger;
    server_messenger = nullptr;

    return 0;
}