//
// Created by EShare on 2019/6/11.
//

#include "at_server.h"

namespace eshare {

    static void* at_server_commander(void* arg) {
        AtServer* at_server = static_cast<AtServer*>(arg);
        ASSERT(at_server != nullptr);
        while (at_server->status() == kThreadRunning) {
            pr_err("process command.....\n");
        }
    }

    AtServer::AtServer() {
        messenger_.registerBusMessageHandler(this);
        messenger_.RegisterSystemMessageHandler(this);
    }

    AtServer::~AtServer() {

    }

    void AtServer::HandleSystemQueryStatusMessage() {

    }

    void AtServer::HandleFaceRecognizedMessage(int user_id, int result) {

    }

    void AtServer::HandleFaceRegisteredMessage(int user_id, int result) {

    }

    void AtServer::start() {
        commander_ = std::make_shared<Thread>("AtServer", at_server_commander, this);
        commander_->join();
    }

    void AtServer::stop() {
        commander_->set_status(kThreadStopping);
    }

}