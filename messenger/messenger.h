#ifndef MESSAGER_H
#define MESSAGER_H

#include <unistd.h>
#include <memory>

#include "ipc.h"
#include "../base/thread.h"

namespace eshare {

    class SystemMessagerHandler {
    public:
        SystemMessagerHandler() {}
        virtual ~SystemMessagerHandler() {};
        virtual void HandleSystemQueryStatusMessage(void) = 0;

    private:

    };

    class Messenger {
    public:
        Messenger() = delete;
        Messenger(const char* name, const char* url);
        virtual ~Messenger();

        typedef std::shared_ptr<Messenger> SharedPtr;

        virtual int SendMessageToBus(std::string& msg) {
            bus_socket_->send(msg);
        }

        virtual int SendMessageToBus(const char* msg) {
            bus_socket_->send(msg);
        }

        virtual int SendMessageToSystem(std::string& msg) {
            system_socket_->send(msg);
        }

        virtual int SendMessageToSystem(const char* msg) {
            system_socket_->send(msg);
        }

        void RegisterSystemMessageHandler(SystemMessagerHandler* handler) {
            system_handler_ = handler;
        }

        virtual int ReceiveSystemMessage(void) {
            std::string message;
            int bytes = system_socket_->receive(message);
            if (bytes < 0) {
                // TODO : wait receive
                return bytes;
            }
            ProcessSystemMessage(message.c_str());
            return 0;
        }

        virtual int ReceiveBusMessage(void) {
            std::string message;
            int bytes = bus_socket_->receive(message);
            if (bytes < 0) {
                // TODO : wait receive
                return bytes;
            }
            ProcessBusMessage(message.c_str());
            return 0;
        }

        virtual int ProcessSystemMessage(const char* string);

        virtual int ProcessBusMessage(const char* msg) = 0;

        ThreadStatus system_message_status(void) const {
            return system_messager_->status();
        }

        ThreadStatus bus_message_status(void) const {
            return bus_messager_->status();
        }

        std::string& name(void) {
            return name_;
        }

        std::string& bind_url(void) {
            return bind_url_;
        }

    private:
        std::string name_;
        std::string bind_url_;
        Socket::SharedPtr system_socket_;
        Socket::SharedPtr bus_socket_;
        Thread::SharedPtr system_messager_;
        Thread::SharedPtr bus_messager_;
        SystemMessagerHandler* system_handler_;

    };    // end of class Messager
}   // end of namespace

#endif
