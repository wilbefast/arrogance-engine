#ifndef WARN_HPP_INCLUDED
#define WARN_HPP_INCLUDED

#include "log.hpp"     // for LOG

#define WARN(what, why)                             \
        log(LOG_WARN, "%s : %s", what, why)

#define WARN_IF(problem, what, why)                 \
    if(problem)                                     \
        WARN(what, why)

#define WARN_RTN(what, why, rtn)                    \
    {                                               \
        WARN(what, why);                             \
        return(rtn);                                \
    }

#endif // WARN_HPP_INCLUDED
