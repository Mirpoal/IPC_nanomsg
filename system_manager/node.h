//
// Created by EShare on 2019/6/10.
//

#ifndef NNMSG_NODE_H
#define NNMSG_NODE_H

#include <unistd.h>
#include <string>
#include <cstring>
#include "../utils/assert.h"

namespace eshare {

    #define MAX_PARAMETER_SIZE 256

    enum class NodeStatus {
        kNodeStopped,
        kNodeRunning,
        kNodeIdle
    };

    class Node {
    public:
        Node() = delete;
        Node(const char* name, const char* path, const char* parameter, int delay = 0) :
            name_(name), path_(path), parameter_(parameter), delay_ms_(delay) {
            status_ = NodeStatus::kNodeStopped;
        }
        virtual ~Node() {};

        virtual int startup(void) {
            pid_t pid = fork();
            ASSERT(pid >= 0);

            if (pid == 0) {
                char* argv[MAX_PARAMETER_SIZE] = {0};
                char params[MAX_PARAMETER_SIZE] = {0};
                int i = 0;

                ASSERT(parameter_.size() < MAX_PARAMETER_SIZE);
                strcpy(params, parameter_.c_str());
                char* arg = strtok(params, " ");
                while (arg != nullptr && i < MAX_PARAMETER_SIZE) {
                    argv[i] = arg;
                    arg = strtok(nullptr, " ");
                    i++;
                }
                execv(path_.c_str(), argv);
            } else {
                status_  = NodeStatus::kNodeRunning;
                pid_ = pid;
                return 0;
            }
        }

        virtual void shutdown(void) {
            char command[128] = {0};
            sprintf(command, "busybox killall %s", name_.c_str());
            system(command);
        }

        std::string name(void) const {
            return name_;
        }

        std::string bind_url(void) const {
            return bind_url_;
        }

        int bus_socket(void) const {
            return bus_socket_;
        }

        int delay_ms(void) const {
            return delay_ms_;
        }

        void set_bind_url(const char* url) {
            bind_url_ = url;
        }

        void set_bus_socket(const int socket) {
            bus_socket_ = socket;
        }

        void set_pid(const int pid) {
            pid_ = pid;
        }

        void set_status(NodeStatus status) {
            status_ = status;
        }

        NodeStatus status(void) const {
            return status_;
        }

    private:
        pid_t pid_;
        std::string name_;
        std::string path_;
        std::string parameter_;

        std::string bind_url_;
        int bus_socket_;
        int delay_ms_;
        NodeStatus status_;
    };

}     // end of namespace


#endif //NNMSG_NODE_H
