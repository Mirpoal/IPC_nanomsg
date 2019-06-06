//
// Created by EShare on 2019/6/10.
//

#include "system_manager.h"
#include "../messenger/json_message.h"
#include "../messenger/system.h"
#include <cjson/cJSON.h>
#include <fstream>
#include <vector>

namespace eshare {

    static void* system_guardian_handler(void* arg) {
        SystemManager* manager = (SystemManager*)arg;
        ASSERT(manager != nullptr);

        while (manager->guardian_status() == kThreadRunning) {
            manager->survey();
            if (manager->system_status() == SystemStatus::kSystemRunning)
                sleep(10);
            else
                usleep(100*1000);
        }
        return nullptr;
    }

    SystemManager::SystemManager() {
        status_ = SystemStatus::kSystemStarting;
        parseConfig();
    }

    SystemManager::~SystemManager() {
        while (!nodes_.empty()) {
            Node* node = nodes_.front();
            nodes_.pop_front();
            delete node;
        }
    }

    int SystemManager::parseConfig() {
        std::ifstream config_file("D:/cygwin64/home/EShare/felix/nanomsg/config/system_manager.conf");
        config_file.seekg(0, config_file.end);
        size_t file_size = config_file.tellg();
        config_file.seekg(0, config_file.beg);

        char* config_stream = (char*)malloc(file_size);
        ASSERT(config_stream != nullptr);
        config_file.read(config_stream, file_size);

        cJSON* config_json = cJSON_Parse(config_stream);
        ASSERT(config_json != nullptr);

        // parse system ipc url
        cJSON* system_url_json = cJSON_GetObjectItemCaseSensitive(config_json, "bind_url");
        ASSERT(system_url_json != nullptr);
        system_socket_ = std::make_shared<Socket>(NN_SURVEYOR);
        system_socket_->bind(system_url_json->valuestring);
        system_socket_->set_receive_timeout(100);

        // parse nodes
        cJSON* nodes_json = cJSON_GetObjectItemCaseSensitive(config_json, "nodes");
        cJSON* node_json = nullptr;
        cJSON_ArrayForEach(node_json, nodes_json) {
            cJSON* name_json = cJSON_GetObjectItemCaseSensitive(node_json, "name");
            cJSON* path_json = cJSON_GetObjectItemCaseSensitive(node_json, "path");
            cJSON* url_json = cJSON_GetObjectItemCaseSensitive(node_json, "bind_url");
            cJSON* param_json = cJSON_GetObjectItemCaseSensitive(node_json, "parameter");
            cJSON* delay_json = cJSON_GetObjectItemCaseSensitive(node_json, "delay_ms");

            Node* node = new Node(name_json->valuestring, path_json->valuestring, param_json->valuestring, delay_json->valuedouble);
            nodes_.push_back(node);
        }

        free(config_stream);
    }

    void SystemManager::start() {

        for (Node* node : nodes_) {
            if (node) {
                node->startup();
                if (node->delay_ms())
                    usleep(node->delay_ms() * 1000);
            }
        }

        guardian_ = std::make_shared<Thread>("systemManager", system_guardian_handler, this);
        guardian_->join();
    }

    void SystemManager::stop() {
        guardian_->set_status(kThreadStopping);
    }

    int SystemManager::requestSystemShutdown() {
        system("poweroff");
        return 0;
    }

    int SystemManager::checkIdleStatus() {
        for (Node* node : nodes_) {
            if (node && node->status() == NodeStatus::kNodeRunning)
                return 0;
        }
        pr_dbg("system is idle\n");
        return 1;
    }

    int SystemManager::restart() {
        pr_err("System needs to restart all process\n");

        for (Node* node : nodes_) {
            if (node)
                node->shutdown();
        }

        for (Node* node : nodes_) {
            if (node) {
                node->startup();
                if (node->delay_ms())
                    usleep(node->delay_ms() * 1000);
            }
        }

        return 0;
    }

    int SystemManager::processResponse(std::string &response) {
        JsonMessage msg(response.c_str());

        if (strcmp(msg.action(), "SystemResponseReady") == 0) {
            cJSON* body = msg.body();
            cJSON* name_json = cJSON_GetObjectItemCaseSensitive(body, "name");
            Node* node = getNodeByName(name_json->valuestring);
            if (!node) {
                return -1;
            }

            cJSON* url_json = cJSON_GetObjectItemCaseSensitive(body, "bind_url");
            node->set_bind_url(url_json->valuestring);
        } else if (strcmp(msg.action(), "SystemResponseStatus") == 0) {
            cJSON* body = msg.body();
            cJSON* name_json = cJSON_GetObjectItemCaseSensitive(body, "name");
            Node* node = getNodeByName(name_json->valuestring);
            if (!node) {
                return -1;
            }
            cJSON* status_json = cJSON_GetObjectItemCaseSensitive(body, "status");
            if (strcmp(status_json->valuestring, "busy") == 0)
                node->set_status(NodeStatus::kNodeRunning);
            else
                node->set_status(NodeStatus::kNodeIdle);
        }
        return 0;
    }

    int SystemManager::survey() {
        if (status_ == SystemStatus::kSystemStarting) {
            SystemQueryReadyMessage msg;
            system_socket_->send(msg.to_string());
        } else if (status_ == SystemStatus::kSystemRunning) {
            SystemQueryStatusMessage msg;
            system_socket_->send(msg.to_string());
        }

        for (int i = 0; i < nodes_.size(); i++) {
            std::string response;
            int bytes = system_socket_->receive(response);
            if (bytes < 0) {
                pr_err("error in system survey receive msg\n");
                return bytes;
            }
            processResponse(response);
        }

        if (status_ == SystemStatus::kSystemStarting) {
            SystemRequestBusConnectionMessage msg;
            std::vector<std::string> url_array;
            for (Node* node : nodes_)
                url_array.push_back(node->bind_url());
            msg.fillBody(url_array);
            system_socket_->send(msg.to_string());
            status_ = SystemStatus::kSystemRunning;
        } else if (status_ == SystemStatus::kSystemRunning) {
            if (checkIdleStatus())
                requestSystemShutdown();
        }
		return 0;
    }
}
