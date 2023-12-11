#ifndef _QUEUE_H_
#define _QUEUE_H_

/*
Simple discrete simulation framework for C++.

My goals with this framework are mainly to make it as general-purpose as possible,
as well as easy to understand and use.
*/

#include <cstddef>

#include "node.h"

namespace des
{
    struct Event
    {
        double time;
        Node* target;
    };

    //Increments the clock. Executes all events in the order designated by their times. Used for discrete time simulation.
    bool advance_time(double time);

    //Advance to the next event in the queue, and execute said event (or multiple events if they're confluent). Used for discrete event simulation.
    bool advance_event();

    //Cleans any memory associated with the framework. Use when terminating the program, or when the framework is no longer in use.
    void clean();

    //Returns the number of events left in the queue.
    size_t events_size();

    //Return the current simulation time.
    double get_clock_time();

    //Pushs an event to the queue.
    void push_event(Event e);

    //Resets and/or initializes all parameters and clears the event queue.
    void reset();
}

#endif