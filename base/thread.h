//
// Created by EShare on 2019/6/6.
//

#ifndef NNMSG_THREAD_H
#define NNMSG_THREAD_H

#include <pthread.h>
#include <string.h>
#include <string>
#include <memory>

namespace eshare {

    typedef void* (*pthread_func_t)(void* arg);

    enum ThreadStatus {
        kThreadUninited = 0,
        kThreadRunning = 1,
        kThreadWaiting = 3,
        kThreadStopping = 4
    };

    class Thread {
    public:
        Thread(const Thread&) = delete;
        Thread(): tid_(0), status_(kThreadUninited) {}
        explicit Thread(const char* name , pthread_func_t func, void* arg);
        virtual ~Thread();

        typedef std::shared_ptr<Thread> SharedPtr;

        bool joinable(void) const {
            return tid_ != 0;
        }

        void join(void) {
            pthread_join(tid_, nullptr);
            tid_ = 0;
        }

        void detach(void) {
            pthread_detach(tid_);
        }

        const std::string& name(void) const {
            return name_;
        }

        void set_status(const ThreadStatus status) {
            status_ = status;
        }

        const ThreadStatus status(void) const {
            return status_;
        }

        friend void* thread_func_wrapper(void* data);

    private:
        static const size_t kThreadStackSize;
        pthread_t tid_;
        ThreadStatus status_;
        std::string name_;
        pthread_func_t func_;
        void* arg_;

    };    // end of class thread
}     // end of namespace

#endif //NNMSG_THREAD_H
