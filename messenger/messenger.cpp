//
// Created by EShare on 2019/6/6.
//

#include <unistd.h>

#include "messenger.h"
#include "../messenger/ipc.h"
#include "../base/thread.h"
#include "../utils/logger.h"
#include "system.h"
#include "json_message.h"

namespace eshare {

    static void* messenger_system_loop(void* arg) {
        Messenger* messenger = (Messenger*)arg;

        while (messenger->system_message_status() == kThreadRunning)
            messenger->ReceiveSystemMessage();

        return nullptr;
    }

    static void* messenger_bus_loop(void* arg) {
        Messenger* messenger = (Messenger*)arg;

        while (messenger->bus_message_status() == kThreadRunning)
            messenger->ReceiveBusMessage();

        return nullptr;
    }

    Messenger::Messenger(const char *name, const char *url) : name_(name), bind_url_(url) {
        system_socket_ = std::make_shared<Socket>(NN_RESPONDENT);
        system_socket_->connect("ipc:///tmp/system.ipc");

        bus_socket_ = std::make_shared<Socket>(NN_BUS);
        bus_socket_->bind(url);

        system_messager_ = std::make_shared<Thread>(name, messenger_system_loop, this);
        bus_messager_ = std::make_shared<Thread>(name, messenger_bus_loop, this);
    }

    Messenger::~Messenger() {
        system_messager_->set_status(kThreadStopping);
        bus_messager_->set_status(kThreadStopping);

        system_messager_->join();
        bus_messager_->join();
    }

    int Messenger::ProcessSystemMessage(const char *string) {
        JsonMessage msg(string);

        if (strcmp(msg.action(), "SystemQueryReady") == 0) {
            SystemResponseReadyMessage msg;
            msg.fillBody(name_.c_str(), bind_url_.c_str());
            SendMessageToSystem(msg.to_string());
        } else if (strcmp(msg.action(), "SystemQueryStatus") == 0) {
            if (system_handler_)
                system_handler_->HandleSystemQueryStatusMessage();
        } else if (strcmp(msg.action(), "SystemRequestBusConnection") == 0) {
            cJSON* body = msg.body();
            cJSON* nodes = cJSON_GetObjectItemCaseSensitive(body, "nodes");
            ASSERT(nodes != nullptr);

            cJSON* node = nullptr;
            std::vector<char*> url_array;
            cJSON_ArrayForEach(node, nodes) {
                cJSON* url = cJSON_GetObjectItemCaseSensitive(node, "url");
                url_array.push_back(url->valuestring);
            }

            for (int i = 0; i < url_array.size(); i++) {
                char* node_url = url_array[i];
                if (node_url == bind_url_) {
                    int next = i + 1;
                    if (next >= url_array.size()) {
                        // To fix duplicate connect when size==2.
                        if (url_array.size() <= 2)
                            break;
                        next = 0;
                    }
                    bus_socket_->connect(url_array[next]);
                    pr_info("%s: connect to %s\n", name_.c_str(), url_array[next]);
                    break;
                }
            }
        }

        return 0;
    }
}
