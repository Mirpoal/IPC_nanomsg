//
// Created by EShare on 2019/6/10.
//

#ifndef NNMSG_SYSTEM_MANAGER_H
#define NNMSG_SYSTEM_MANAGER_H

#include <list>
#include "../messenger/service.h"
#include "../base/thread.h"
#include "node.h"
#include "../messenger/ipc.h"

namespace eshare {

    enum class SystemStatus {
        kSystemStarting,
        kSystemRunning,
        kSystemStopping
    };

    class SystemManager : public Service {
    public:
        SystemManager();
        virtual ~SystemManager();

        virtual void start(void) override;
        virtual void stop(void) override;

        ThreadStatus guardian_status(void) {
            if (guardian_) {
                return guardian_->status();
            } else
                return kThreadUninited;
        }

        SystemStatus system_status(void) const {
            return status_;
        }

        Node* getNodeByName(const char* name) {
            for (Node* node : nodes_) {
                if (node->name() == name) {
                    return node;
                }
            }

            return nullptr;
        }

        void waitingNodeConnection(void);

        int restart(void);
        int requestSystemShutdown(void);
        int checkIdleStatus(void);
        int processResponse(std::string& response);
        int survey(void);
        int parseConfig(void);

    private:
        SystemStatus status_;
        Thread::SharedPtr guardian_;
        Socket::SharedPtr system_socket_;
        std::list<Node*> nodes_;
    };
}


#endif //NNMSG_SYSTEM_MANAGER_H
