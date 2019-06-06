#ifndef _IPC_H_
#define _IPC_H_

#include <string.h>

#include <list>
#include <memory>

#include <nanomsg/nn.h>
#include <nanomsg/survey.h>
#include <nanomsg/bus.h>
#include <nanomsg/pair.h>

#include "../utils/assert.h"

namespace eshare {

    class Connection {
    public:
        enum class Type { Connect, Bind };

        Connection(int socket_id, std::string const& url, Type type)
                : socket_id_(socket_id)
                , connection_id_(type == Type::Connect ? nn_connect(socket_id_, url.c_str()) : nn_bind(socket_id_, url.c_str()))
        {
            ASSERT(socket_id_ >= 0);
            //ASSERT(connection_id_ >= 0);
        }

        ~Connection() {
            nn_shutdown(socket_id_, connection_id_);
        }

        typedef std::shared_ptr<Connection> SharedPtr;

    private:
        Connection(const Connection&) = delete;
        Connection& operator=(const Connection&) = delete;

        const int socket_id_;
        const int connection_id_;

    };   // end of class Connection

    class Socket {
    public:
        Socket(int type): socket_id_(nn_socket(AF_SP, type)) {
            ASSERT(socket_id_ >= 0);
        }

        ~Socket() {
            while (connections_.size()) {
                Connection::SharedPtr connection = connections_.back();
                connections_.pop_back();
                connection.reset();
            }
            nn_close(socket_id_);
        }

        typedef std::shared_ptr<Socket> SharedPtr;

        void bind(const std::string& url) {
            Connection::SharedPtr connection = std::make_shared<Connection>(socket_id_, url, Connection::Type::Bind);
            connections_.push_back(connection);
        }

        void connect(const std::string& url) {
            Connection::SharedPtr connection = std::make_shared<Connection>(socket_id_, url, Connection::Type::Connect);
            connections_.push_back(connection);
        }

        int send(std::string& message) {
            ASSERT(connections_.size() > 0);
            uint bytes = nn_send(socket_id_, message.c_str(), message.size() + 1, 0);
            if (bytes < 0)
                pr_err("Send messenger failed, errno = %d\n", errno);

            return bytes;
        }

        int send(const char* message) {
            ASSERT(connections_.size() > 0);
            uint bytes = nn_send(socket_id_, message, strlen(message) + 1, 0);
            if (bytes < 0)
                pr_err("Send messenger failed, errno = %d\n", errno);

            return bytes;
        }

        int receive(std::string& string) {
            ASSERT(connections_.size() > 0);

            char* buf = nullptr;
            int bytes = nn_recv(socket_id_, &buf, NN_MSG, 0);
            if (bytes > 0) {
                string = buf;
                nn_freemsg(buf);
			} else
                pr_err("Receive messenger failed, errno = %d\n", errno);
            return bytes;
        }

        void set_receive_timeout(int timeout) {
            nn_setsockopt(socket_id_, NN_SOL_SOCKET, NN_RCVTIMEO, &timeout, sizeof(timeout));
        }

        void set_send_timeout(int timeout) {
            nn_setsockopt(socket_id_, NN_SOL_SOCKET, NN_SNDTIMEO, &timeout, sizeof(timeout));
        }

        bool is_Connectd(void) const {
            if (connections_.size() >= 2) {
                return true;
            } else
                return false;
        }

        int socket_id(void) {
            return socket_id_;
        }

    private:

        Socket(const Socket&) = delete;
        Socket& operator=(const Socket&) = delete;

        const int socket_id_;
        std::list<Connection::SharedPtr> connections_;
    };    // end of class Socket

}   // end of namespace eshare

#endif

