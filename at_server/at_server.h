//
// Created by EShare on 2019/6/11.
//

#ifndef NNMSG_AT_SERVER_H
#define NNMSG_AT_SERVER_H

#include "../base/thread.h"
#include "../messenger/service.h"
#include "../messenger/messenger.h"
#include "at_messenger.h"

namespace eshare {

    enum AtServerMode {
        kAtServerDataMode,
        kAtServerCommandMode,
    };

    class AtServer : public Service, SystemMessagerHandler, BusMessageHander {
    public:
        AtServer();

        virtual ~AtServer();

        virtual void start(void) override;
        virtual void stop(void) override;

        virtual void HandleSystemQueryStatusMessage() override;

        virtual void HandleFaceRegisteredMessage(int user_id, int result) override;
        virtual void HandleFaceRecognizedMessage(int user_id, int result) override;

        ThreadStatus status(void) {
            if (commander_)
                return commander_->status();
            else
                return kThreadUninited;
        }

    private:
        AtServerMode mode_;
        Thread::SharedPtr commander_;
        AtServerMessenger messenger_;
        std::string status_;
    };
}


#endif //NNMSG_AT_SERVER_H
