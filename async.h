#pragma once
#include "taskList.cpp"

typedef ListNode TaskId; 

class Async {
    public:
        Async(unsigned long (*timingFunction)()): _timingFunction(timingFunction) {}
        TaskId* setTimeout(void (*_callback)(), unsigned long _timeout);
        TaskId* setInterval(void (*_callback)(), unsigned long _interval);
        void clearInterval(TaskId*);
        void loop();
    private:
        TaskList _taskList;
        unsigned long (*_timingFunction)();
};
