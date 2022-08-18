struct Task {
    void (*execute)();
    unsigned long time;
    unsigned long interval;
    Task(void (*_function)(), unsigned long _time):
        execute(_function), time(_time) {}
    Task(void (*_function)(), unsigned long _time, unsigned long _interval):
        execute(_function), time(_time), interval(_interval) {}
};

struct ListNode {
    Task value;
    ListNode* next;
    ListNode(Task _task): value(_task), next(nullptr) {}
};

struct TaskList {
    ListNode* first;
    ListNode* last;
    int length = 0;

    TaskList(): first(nullptr), last(nullptr) {}

    ListNode* push(Task _task) {
        ListNode* pointer = new ListNode(_task);
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

    void remove(int index) {
        ListNode* beforeTarget = first;
        if (index == 0) {
            first = beforeTarget -> next;
            delete beforeTarget;
            length--;
            return;
        }
        for (int i = 0; i < index - 1; i++) {
            beforeTarget = beforeTarget -> next;
        }
        ListNode* target = beforeTarget -> next;
        beforeTarget -> next = target -> next;
        delete target;
        length--;
    }

    void remove(ListNode* pointer) {
        if (first == nullptr) return;
        ListNode* beforeTarget = first;
        if (beforeTarget == pointer) {
            first = beforeTarget -> next;
            delete beforeTarget;
            length--;
            return;
        }
        while (beforeTarget -> next != pointer) {
            beforeTarget = beforeTarget -> next;
            if (beforeTarget == nullptr) return;
        }
        ListNode* target = beforeTarget -> next;
        beforeTarget -> next = target -> next;
        delete target;
        length--;
    }

    ListNode* operator[](const int index) {
        if (first == nullptr) return nullptr;
        ListNode* res = first;
        for (int i = 0; i < index; i++) {
            res = res -> next;
            if (res == nullptr) return nullptr;
        }
        return res;
    }

    void forEach(void (*_callback)(Task&, int, TaskList&, unsigned long (*timingFunction)()), unsigned long (*timingFunction)()) {
        if (first == nullptr) return;
        ListNode* node = first;
        for (int i = 0; i < length; i++) {
            _callback(node -> value, i, *this, timingFunction);
            node = node -> next;
        }
    }
};
