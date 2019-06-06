//
// Created by EShare on 2019/6/11.
//

#ifndef NNMSG_AT_MESSENGER_H
#define NNMSG_AT_MESSENGER_H

#include "../messenger/messenger.h"
#include "../messenger/json_message.h"
#include <memory>

namespace eshare {

    class BusMessageHander {
    public:
        BusMessageHander() {};
        virtual ~BusMessageHander() {}
        virtual void HandleFaceRegisteredMessage(int user_id, int result) = 0;
        virtual void HandleFaceRecognizedMessage(int user_id, int result) = 0;
    private:

    };

    class AtServerMessenger : public Messenger {
    public:
        AtServerMessenger() : Messenger("AtServer", "ipc:///tmp/at_server.ipc") {}
        virtual ~AtServerMessenger() {}

        typedef std::shared_ptr<AtServerMessenger> SharedPtr;

        virtual int ProcessBusMessage(const char* msg) override {
            JsonMessage jsonmsg(msg);
            if (strcmp(jsonmsg.action(), "FaceRegistered") == 0) {
                cJSON* body = jsonmsg.body();
                cJSON* user_id = cJSON_GetObjectItemCaseSensitive(body, "user_id");
                cJSON* result = cJSON_GetObjectItemCaseSensitive(body, "result");

                if (bus_handler_)
                    bus_handler_->HandleFaceRegisteredMessage(user_id->valuedouble, result->valuedouble);
            } else if (strcmp(jsonmsg.action(), "FaceRecognized") == 0) {
                cJSON* body = jsonmsg.body();
                cJSON* user_id = cJSON_GetObjectItemCaseSensitive(body, "user_id");
                cJSON* result = cJSON_GetObjectItemCaseSensitive(body, "result");
                if (bus_handler_)
                    bus_handler_->HandleFaceRecognizedMessage(user_id->valuedouble, result->valuedouble);
            }

            return 0;
        }

        virtual void registerBusMessageHandler(BusMessageHander* handler) {
            bus_handler_ = handler;
        }

    private:
        BusMessageHander* bus_handler_;
    };
}


#endif //NNMSG_AT_MESSENGER_H
