#include <map>
#include <ctime>
#include <unistd.h>
#include <thread>
#include "Scheduler.hpp"

using namespace socialine::util::task;

std::function<void(void)> null_routine = [](){};

//-------------------------------------------------------------------------
//		Attributes
//-------------------------------------------------------------------------
std::map<long, bool> Scheduler::intervalRoutines = std::map<long, bool>();
std::map<long, bool> Scheduler::timeoutRoutine = std::map<time_t, bool>();
std::function<void(void)>& Scheduler::currentRoutine = null_routine;
long Scheduler::currentRoutineId;
pthread_t Scheduler::controlThread;


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
long Scheduler::set_interval(const std::function<void(void)>& routine, long interval)
{
    initialize_routine_id();
    currentRoutine = routine;
  
    pthread_t thread;
    pthread_create(&thread, nullptr, interval_control_routine, (void*) interval);

    return currentRoutineId;
}

void Scheduler::initialize_routine_id()
{
    currentRoutineId = get_current_time();
}

void* Scheduler::interval_control_routine(void* arg)
{
    long interval = (long) arg;
    long id = currentRoutineId;
    const std::function<void(void)>& routine = currentRoutine;

    intervalRoutines[id] = true;

    while (intervalRoutines[id])
    {
        routine();
        usleep(interval * 1000);
    }
}

void Scheduler::clear_interval(long id)
{
    intervalRoutines[id] = false;
}

bool Scheduler::set_timeout_to_routine(const std::function<void(void)>& routine, long timeout)
{
    run_routine(routine);
    wait_routine_for(timeout);
    finish_routine();

    return !has_routine_finished();
}

void Scheduler::run_routine(const std::function<void(void)>& routine)
{
    initialize_routine_id();
    
    timeoutRoutine[currentRoutineId] = false;
    currentRoutine = routine;
    
    pthread_create(&controlThread, nullptr, control_routine, nullptr);
}

time_t Scheduler::get_current_time()
{
    return std::time(nullptr);
}

void* Scheduler::control_routine(void* args)
{
    time_t id = currentRoutineId;
    std::function<void(void)>& routine = currentRoutine;
    
    routine();
    timeoutRoutine[id] = true;
}

void Scheduler::wait_routine_for(long time)
{
    time_t id = currentRoutineId;
    time_t start = get_current_time();

    while ((time_elapsed_in_milliseconds(start) < (double) time) && !has_routine_finished())
    {
        usleep(200 * 1000);
    }
}

double Scheduler::time_elapsed_in_milliseconds(time_t start)
{
    return (difftime(get_current_time(), start) * 1000);
}

bool Scheduler::has_routine_finished()
{
    return timeoutRoutine[currentRoutineId];
}

void Scheduler::finish_routine()
{
    pthread_cancel(controlThread);
}

