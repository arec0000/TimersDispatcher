// перенёс всё в один файл, потому что не нашёл другого способа работать с шаблонами

// *переполнение не учитывается, нельзя указывать интервал больше maxTime
// можно сделать облегчённую версию, без учёта переполнения, на случай, если не нужно

#pragma once
#include "taskList.cpp"

typedef ListNode TaskId;

template <typename TimeType>
class Async {

    public:
        Async(TimeType (*timingFunction)(), TimeType maxTime):
            _timingFunction(timingFunction), _maxTime(maxTime) {}

        TaskId* setTimeout(void (*callback)(), TimeType timeout) {
            return _taskList.push(_makeTask(callback, timeout, true));
        }

        TaskId* setInterval(void (*callback)(), TimeType interval) {
            return _taskList.push(_makeTask(callback, interval, false));
        }

        void clearInterval(TaskId* pointer) {
            _taskList.remove(pointer);
        }

        void loop() {
            if (_taskList.first == nullptr) return;
            _taskList.forEach(Async::_checkTask, _timingFunction, _maxTime, _lastTime);
        }

    private:
        TaskList<TimeType> _taskList;
        TimeType (*_timingFunction)();
        TimeType _maxTime;
        TimeType _lastTime;

        Task<TimeType> _makeTask(void (*callback)(), TimeType interval, bool once) {
            Task<TimeType> task(callback, interval, once);
            if (_timingFunction() > _maxTime - interval) {
                task.time = _timingFunction() - (_maxTime - interval);
                task.isBlocked = true;
            } else {
                task.time = _timingFunction() + interval;
            }
            return task;
        }

        static bool _checkTask(
                Task<TimeType>& task, 
                TimeType (*timingFunction)(), 
                TimeType maxTime,
                bool timeOverflowStage
            ) {
            if (!task.isBlocked && (timingFunction() > task.time || timeOverflowStage)) {
                task.execute();
                if (task.once) {
                    return true;
                }
                TimeType time = timingFunction();
                if (time > maxTime - task.interval) {
                    task.time = time - (maxTime - task.interval);
                    task.isBlocked = true;
                } else {
                    task.time = time + task.interval;
                }
            } else if (timeOverflowStage) {
                task.isBlocked = false;
            }
            return false;
        }

};
