#include "TimersDispatcher.h"
#include "EventEmitter.h"

template <typename TimeType>
class Async: public TimersDispatcher<TimeType>, public EventEmitter {
    public:
        Async(TimeType (*timingFunction)(), TimeType maxTime):
            TimersDispatcher<TimeType>(timingFunction, maxTime) {}
};
