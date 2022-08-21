struct EventListener {
    const char* eventName;
    void (*execute)();
    bool once;
    EventListener(const char* _eventName, void (*_callback)(), bool _once):
        eventName(_eventName), execute(_callback), once(_once) {}
};

struct EventNode {
    EventListener value;
    EventNode* next;
    EventNode(EventListener listener): value(listener), next(nullptr) {}
};

struct EventsList {
    EventNode* first;
    EventNode* last;
    unsigned int length = 0;

    EventsList(): first(nullptr), last(nullptr) {}

    void push(EventListener listener) {
        EventNode* pointer = new EventNode(listener);
        if (first != nullptr) {
            last -> next = pointer;
            last = pointer;
        } else {
            first = pointer;
            last = pointer;
        }
        length++;
    }

    void remove(const char* eventName, void (*callback)()) {
        if (first == nullptr) return;
        if (first -> value.eventName == eventName && first -> value.execute == callback) {
            removeFirst();
            return;
        }
        EventNode* before = first;
        while (before -> next -> value.eventName != eventName || before -> next -> value.execute != callback) {
            before = before -> next;
            if (before == nullptr) return;
        }
        removeNext(before);
    }

    void removeFirst() {
        EventNode* target = first;
        first = target -> next;
        delete target;
        length--;
    }

    void removeNext(EventNode* before) {
        EventNode* target = before -> next;
        before -> next = target -> next;
        delete target;
        length--;
    }
};
