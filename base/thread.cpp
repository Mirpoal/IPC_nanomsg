//
// Created by EShare on 2019/6/6.
//

#include "thread.h"
#include "../utils/logger.h"
//#include <sys/prctl.h>

namespace eshare {
    const size_t Thread::kThreadStackSize = 128 * 1024;

    void* thread_func_wrapper(void* data) {
        Thread* thread = (Thread*)data;

        //prctl(PR_SET_NAME, thread->name().c_str(), 0, 0, 0);
        return thread->func_(thread->arg_);
    }

    Thread::Thread(const char *name, eshare::pthread_func_t func, void *arg) : name_(name), func_(func),
        arg_(arg), tid_(0), status_(kThreadUninited) {
        pthread_attr_t attr;

        if (pthread_attr_init(&attr)) {
            pr_err("pthread_attr_init failed!\n");
            return;
        }

        if (pthread_attr_setstacksize(&attr, Thread::kThreadStackSize)) {
            pr_err("pthread_attr_setstacksize failed!\n");
            goto out;
        }
        status_ = kThreadRunning;
        if (pthread_create(&tid_, &attr, thread_func_wrapper, this) != 0) {
            status_ = kThreadUninited;
            pr_err("pthread_create failed!\n");
        }

    out:
        if (pthread_attr_destroy(&attr)) {
            pr_err("pthread_attr_destroy failed!\n");
        }
    }

    Thread::~Thread() {
        if (joinable()) {
            std::terminate();
        }
    }

}    // end of namespace
