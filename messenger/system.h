//
// Created by EShare on 2019/6/10.
//

#ifndef NNMSG_SYSTEM_H
#define NNMSG_SYSTEM_H

#include "json_message.h"
#include <vector>

namespace eshare {

    /**
     * system query ready message
     */
    class SystemQueryReadyMessage : public JsonMessage {
    public:
        SystemQueryReadyMessage() : JsonMessage("any", "Survey", "SystemQueryReady") {}
        virtual ~SystemQueryReadyMessage() {};
    };

    /**
     * system response ready message
     */
    class SystemResponseReadyMessage : public JsonMessage {
    public:
        SystemResponseReadyMessage() : JsonMessage("any", "Survey", "SystemResponseReady") {}
        virtual ~SystemResponseReadyMessage() {};

        void fillBody(const char* name, const char* url) {
            cJSON* body = cJSON_CreateObject();
            ASSERT(body != nullptr);

            cJSON_AddStringToObject(body, "name", name);
            cJSON_AddStringToObject(body, "bind_url", url);

            addBody(body);
        }
    };

    /**
     * system query status message
     */
    class SystemQueryStatusMessage : public JsonMessage {
    public:
        SystemQueryStatusMessage() : JsonMessage("any", "Survey", "SystemQueryStatus") {}
        virtual ~SystemQueryStatusMessage() {};
    };

    /**
     * system response status message
     */
    class SystemResponseStatusMessage : public JsonMessage {
    public:
        SystemResponseStatusMessage() : JsonMessage("any", "Survey", "SystemResponseStatus") {}
        virtual ~SystemResponseStatusMessage() {};

        void fillBody(const char* name, const char* status) {
            cJSON* body = cJSON_CreateObject();
            ASSERT(body != nullptr);

            cJSON_AddStringToObject(body, "name", name);
            cJSON_AddStringToObject(body, "status", status);

            addBody(body);
        }
    };

    class SystemRequestBusConnectionMessage : public JsonMessage {
    public:
        SystemRequestBusConnectionMessage() : JsonMessage("any", "Survey", "SystemRequestBusConnection") {}
        virtual ~SystemRequestBusConnectionMessage() {};

        void fillBody(std::vector<std::string>& url_array) {
            cJSON* body = cJSON_CreateObject();
            ASSERT(body != nullptr);

            cJSON* url_array_json = cJSON_CreateArray();

            for (int i = 0; i < url_array.size(); ++i) {
                std::string& url = url_array[i];
                cJSON* url_json = cJSON_CreateObject();
                cJSON_AddStringToObject(url_json, "url", url.c_str());
                cJSON_AddItemToArray(url_array_json, url_json);
            }

            cJSON_AddItemToObject(body, "nodes", url_array_json);
            addBody(body);
        }
    };

}     // end of namespace

#endif //NNMSG_SYSTEM_H
