#include "queue.h"

#include <iostream>
#include <vector>

static bool queue_initialized = false;

static std::vector<des::Event>  events;
static std::vector<double> event_times;

static size_t queue_size;

static double queue_clock;

//---------------------------------------------------------
//                          Queue
//---------------------------------------------------------

//Implemeting a binary heap.
class queue
{
    public:
        queue() = delete;

        static des::Event get(size_t offset);

        static void heapify(size_t index);

        static void insert(des::Event e);
        static void pop();

        static void reset();

        static size_t size() { return queue_size; }
        static void swap(size_t index_a, size_t index_b);
};

des::Event queue::get(size_t offset)
{
    if(queue_size == 0 || offset >= queue_size) return des::Event();
	else return events[offset];
}

void queue::heapify(size_t index)
{
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;
    
    size_t smallest = index;
    
    if(left < queue_size && event_times[left] <= event_times[smallest])
    {
        smallest = left;
    }
    
    if(right < queue_size && event_times[right] <= event_times[smallest])
    {
        smallest = right;
    }
    
    if(smallest != index)
    {
        swap(index, smallest);
        heapify(smallest);
    }
}

void queue::insert(des::Event e)
{
    queue_size++;
    events.push_back(e);
    event_times.push_back(e.time);
    heapify(queue_size - 1);
}

void queue::pop()
{
    events[0] = events[queue_size - 1];
    event_times[0] = event_times[queue_size - 1];

    events.pop_back();
    event_times.pop_back();

    queue_size--;
    heapify(0);
}

void queue::reset()
{
    if(queue_initialized)
    {
        //Using clear() doesn't deallocate memory when clearing the vector, while this trick does. You can also use shrink_to_fit() after clear() starting in C++11.
        std::vector<des::Event>().swap(events);
        std::vector<double>().swap(event_times);
    }
    queue_size = 0;
    queue_initialized = true;
}

void queue::swap(size_t index_a, size_t index_b)
{
    des::Event temp = events[index_b];
    events[index_b] = events[index_a];
    events[index_a] = temp;

    double t = event_times[index_b];
    event_times[index_b] = event_times[index_a];
    event_times[index_a] = t;
}

//---------------------------------------------------------
//                    Public functions
//---------------------------------------------------------

bool des::advance_time(double time)
{
    if(queue::size() == 0)
    {
        queue_clock += time;
        return false;
    }
    
    Event next = queue::get(0);

    double time_next = next.time;
    double time_advance = time + queue_clock;

    while(time_next <= time_advance)
    {
        queue_clock = time_next;

        size_t start = 0;
        size_t end;

        //Checking for confluent events.
        for(size_t i = start + 1; i <= queue::size(); i++)
        {
            if(i == queue::size() || time_next != queue::get(i).time)
            {
                end = i;
                break;
            }
        }

        //Execute all lambdas.
        for(size_t i = start; i < end; i++)
        {
            queue::get(i).target->lambda(queue_clock);
        }

        //Execute all deltas.
        for(size_t i = start; i < end; i++)
        {
            queue::get(i).target->delta();
        }

        //Pop events.
        for(size_t i = start; i < end; i++)
        {
            queue::pop();
        }

        if(queue::size() > 0)
        {
            next = queue::get(0);
            time_next = next.time;
        }
        else
        {
            break;
        }
    }

    queue_clock = time_advance;

    return true;
}

bool des::advance_event()
{
    if(queue::size() == 0) return false;

    Event next = queue::get(0);

    size_t start = 0;
    size_t end;

    double time = next.time;
    queue_clock = time;

    //Checking for confluent events.
    for(size_t i = start + 1; i <= queue::size(); i++)
    {
        if(i == queue::size() || time != queue::get(i).time)
        {
            end = i;
            break;
        }
    }

    //Execute all lambdas.
    for(size_t i = start; i < end; i++)
    {
        queue::get(i).target->lambda(queue_clock);
    }

    //Execute all deltas.
    for(size_t i = start; i < end; i++)
    {
        queue::get(i).target->delta();
    }

    //Pop events.
    for(size_t i = start; i < end; i++)
    {
        queue::pop();
    }

    return true;
}

size_t des::events_size()
{
    return queue::size();
}

double des::get_clock_time()
{
    return queue_clock;
}

void des::reset()
{
    queue_clock = 0;
    queue::reset();

    queue_initialized = true;
}

void des::push_event(Event e)
{
    queue::insert(e);
}