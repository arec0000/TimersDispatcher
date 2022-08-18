#include "async.h"

static void checkTask(Task& task, int i, TaskList& list, unsigned long (*timingFunction)()) {
    if (timingFunction() > task.time) {
        task.execute();
        if (task.interval > 0) {
            task.time = timingFunction() + task.interval;
        } else {
            list.remove(i);
        }
    }
}

TaskId* Async::setTimeout(void (*_callback)(), unsigned long _timeout) {
    return _taskList.push(Task(_callback, _timingFunction() + _timeout));
}

TaskId* Async::setInterval(void (*_callback)(), unsigned long _interval) {
    return _taskList.push(Task(_callback, _timingFunction() + _interval, _interval));
}

void Async::clearInterval(TaskId* pointer) {
    _taskList.remove(pointer);
}

void Async::loop() {
    _taskList.forEach(checkTask, _timingFunction);
}
