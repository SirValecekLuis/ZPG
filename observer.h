//
// Created by tobiasjanca on 10/20/24.
//

#ifndef OBSERVER_H
#define OBSERVER_H


class Subject;

class Observer {
public:
    virtual ~Observer() = default;

    virtual void update(Subject *subject) = 0;
};

class Subject {
public:
    virtual ~Subject() = default;

    virtual void add_observer(Observer *observer) = 0;

    virtual void remove_observer(Observer *observer) = 0;

    virtual void notify_observers() = 0;
};


#endif //OBSERVER_H
