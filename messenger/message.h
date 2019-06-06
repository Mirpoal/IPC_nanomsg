//
// Created by EShare on 2019/6/6.
//

#ifndef NNMSG_MESSAGE_H
#define NNMSG_MESSAGE_H

#include <memory>
#include <string>

namespace eshare {

    class Message {
    public:
        Message() {};
        virtual ~Message() {};

        typedef std::shared_ptr<Message> MessagePtr;

        virtual const char* to_string(void) {
            return string_;
        }

    protected:
        char* string_;
    };
}

#endif //NNMSG_MESSAGE_H
