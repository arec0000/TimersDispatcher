#pragma once
#include "eventsList.cpp"

class EventEmitter {
    public:
        void on(const char* eventName, void (*callback)()) {
            _eventsList.push(EventListener(eventName, callback, false));
        }

        void once(const char* eventName, void (*callback)()) {
            _eventsList.push(EventListener(eventName, callback, true));
        }

        void removeListener(const char* eventName, void (*callback)()) {
            _eventsList.remove(eventName, callback);
        }

        void removeAllListeners(const char* eventName) {
            if (_eventsList.first == nullptr) return;
            EventNode* before = _eventsList.first;
            EventNode* node = _eventsList.first;
            while (node != nullptr) {
                if (node -> value.eventName == eventName) {
                    if (node == _eventsList.first) {
                        _eventsList.removeFirst();
                        before = _eventsList.first;
                        node = _eventsList.first;
                    } else {
                        _eventsList.removeNext(before);
                        node = before -> next;
                    }
                } else {
                    if (node == _eventsList.first) {
                        node = node -> next;
                    } else {
                        before = node;
                        node = node -> next;
                    }
                }
            }
        }

        void removeAllListeners() {
            while (_eventsList.first != nullptr) {
                _eventsList.removeFirst();
            }
        }

        void emit(const char* eventName) { //передавать аргумет
            if (_eventsList.first == nullptr) return;
            EventNode* before = _eventsList.first;
            EventNode* node = _eventsList.first;
            while (node != nullptr) {
                if (node -> value.eventName == eventName) {
                    node -> value.execute();
                    if (node -> value.once) {
                        if (node == _eventsList.first) {
                            _eventsList.removeFirst();
                            before = _eventsList.first;
                            node = _eventsList.first;
                        } else {
                            _eventsList.removeNext(before);
                            node = before -> next;
                        }
                        continue;
                    }
                }
                if (node == _eventsList.first) {
                    node = node -> next;
                } else {
                    before = node;
                    node = node -> next;
                }
            }
        }

    private:
        EventsList _eventsList;
};
