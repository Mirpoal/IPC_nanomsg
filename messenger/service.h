//
// Created by EShare on 2019/6/10.
//

#ifndef NNMSG_SERVICE_H
#define NNMSG_SERVICE_H

namespace eshare {

    class Service {
    public:
        Service() {};
        virtual ~Service() {};

        virtual void start(void) = 0;
        virtual void stop(void) = 0;

        bool is_running(void) const {
            return is_running_;
        }

    private:
        bool is_running_;
    };
}

#endif //NNMSG_SERVICE_H
