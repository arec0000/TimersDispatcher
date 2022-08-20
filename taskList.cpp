template <typename TimeType>
struct Task {
    void (*execute)();
    TimeType time;
    TimeType interval;
    bool once;
    bool isBlocked = false;
    Task(void (*_function)(), TimeType _interval, bool _once):
        execute(_function), interval(_interval), once(_once) {}
};

struct ListNode {};

template <typename TimeType>
struct TaskNode : ListNode {
    Task<TimeType> value;
    TaskNode<TimeType>* next;
    TaskNode(Task<TimeType> _task): value(_task), next(nullptr) {}
};

template <typename TimeType>
struct TaskList {
    public:
        TaskNode<TimeType>* first;
        TaskNode<TimeType>* last;
        unsigned int length = 0;

        TaskList(): first(nullptr), last(nullptr) {}

        TaskNode<TimeType>* push(Task<TimeType> task) {
            TaskNode<TimeType>* pointer = new TaskNode<TimeType>(task);
            if (first != nullptr) {
                last -> next = pointer;
                last = pointer;
            } else {
                first = pointer;
                last = pointer;
            }
            length++;
            return pointer;
        }

        void remove(ListNode* pointer) {
            if (first == nullptr) return;
            if (pointer == first) {
                _removeFirst();
                return;
            }
            TaskNode<TimeType>* beforeTarget = first;
            while (beforeTarget -> next != pointer) {
                beforeTarget = beforeTarget -> next;
                if (beforeTarget == nullptr) return;
            }
            _remove(beforeTarget);
        }

        void forEach( // кажется весь этот функционал стоит перенести в класс Async
                bool (*_callback)(Task<TimeType>&, TimeType (*)(), TimeType, bool), 
                TimeType (*timingFunction)(),
                TimeType maxTime,
                TimeType& lastTime
            ) {
            TaskNode<TimeType>* before = first;
            TaskNode<TimeType>* node = first;
            bool timeOverflowStage = false;
            while (node != nullptr) {
                ///// странно, что я отслеживаю это здесь, но это самый удобный момент
                TimeType time = timingFunction();
                if (lastTime > time) {
                    before = first;
                    node = first;
                    timeOverflowStage = true;
                }
                lastTime = time;
                /////
                if (node == first) {
                    bool isFinished = _callback(node -> value, timingFunction, maxTime, timeOverflowStage);
                    if (isFinished) {
                        _removeFirst();
                        node = first;
                        before = first;
                    } else {
                        node = node -> next;
                    }
                } else {
                    bool isFinished = _callback(node -> value, timingFunction, maxTime, timeOverflowStage);
                    if (isFinished) {
                        _remove(before);
                        node = before -> next;
                    } else {
                        before = node;
                        node = node -> next;
                    }
                }
            }
        }

        private:
            void _removeFirst() {
                TaskNode<TimeType>* target = first;
                first = target -> next;
                delete target;
                length--;
            }

            void _remove(TaskNode<TimeType>* before) {
                TaskNode<TimeType>* target = before -> next;
                before -> next = target -> next;
                delete target;
                length--;
            }
};

// void remove(int index) {
//     TaskNode<TimeType>* beforeTarget = first;
//     if (index == 0) {
//         first = beforeTarget -> next;
//         delete beforeTarget;
//         length--;
//         return;
//     }
//     for (int i = 0; i < index - 1; i++) {
//         beforeTarget = beforeTarget -> next;
//     }
//     TaskNode<TimeType>* target = beforeTarget -> next;
//     beforeTarget -> next = target -> next;
//     delete target;
//     length--;
// }

// TaskNode<TimeType>* operator[](const int index) {
//     if (first == nullptr) return nullptr;
//     TaskNode<TimeType>* res = first;
//     for (int i = 0; i < index; i++) {
//         res = res -> next;
//         if (res == nullptr) return nullptr;
//     }
//     return res;
// }
