//
// Created by EShare on 2019/6/6.
//

#ifndef NNMSG_JSON_MESSAGE_H
#define NNMSG_JSON_MESSAGE_H

#include <unistd.h>
#include <cjson/cJSON.h>
#include "../utils/assert.h"
#include "message.h"

namespace eshare {

    class JsonMessage : public Message {
    public:
        JsonMessage() = delete;
        JsonMessage(const char* json) {
            json_ = cJSON_Parse(json);
            ASSERT(json_);
            string_ = nullptr;
        }

        JsonMessage(const char* to, const char* type, const char* action) {
            string_ = nullptr;
            json_ = cJSON_CreateObject();

            ASSERT(json_ != nullptr);
            cJSON_AddNumberToObject(json_, "from", getpid());
            cJSON_AddStringToObject(json_, "to", to);
            cJSON_AddStringToObject(json_, "type", type);
            cJSON_AddStringToObject(json_, "action", action);
        }

        ~JsonMessage() {
            cJSON_Delete(json_);
            if (string_) {
                free(string_);
                string_ = nullptr;
            }
        }

        int from(void) const {
            cJSON* from = cJSON_GetObjectItemCaseSensitive(json_, "from");
            if (from)
                return from->valueint;
            else
                return -1;
        }

        const char* to(void) const {
            cJSON* to = cJSON_GetObjectItemCaseSensitive(json_, "to");
            if (cJSON_IsString(to) && to->valuestring != nullptr)
                return to->valuestring;
            else
                return nullptr;
        }

        const char* type(void) const {
            cJSON* type = cJSON_GetObjectItemCaseSensitive(json_, "type");
            if (cJSON_IsString(type) && type->valuestring != nullptr)
                return type->valuestring;
            else
                return nullptr;
        }

        const char* action(void) const {
            cJSON* action = cJSON_GetObjectItemCaseSensitive(json_, "action");
            if (cJSON_IsString(action) && action->valuestring != nullptr)
                return action->valuestring;
            else
                return nullptr;
        }

        cJSON* body(void) {
            return cJSON_GetObjectItemCaseSensitive(json_, "body");
        }

        virtual void addBody(cJSON* body) {
            cJSON_AddItemToObject(json_, "body", body);
        }

        virtual const char* to_string(void) override {
            if (string_ == nullptr) {
                string_ = cJSON_Print(json_);
            }
            return string_;
        }

    private:
        cJSON* json_;
    };

}     // end of namespace

#endif //NNMSG_JSON_MESSAGE_H
