#ifndef NNMSG_ASSERT_H_
#define NNMSG_ASSERT_H_

#include <assert.h>
#include "logger.h"

#ifdef DEBUG
#define ASSERT(s) assert(s)
#else
#define ASSERT(s)   \
	do {            \
		if (!(s))     \
			eshare::Logger::Error(__func__, "assert : line = %d", __LINE__);   \
	} while(0)
#endif

#endif    // UTILS_ASSERT

