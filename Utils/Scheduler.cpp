#include <map>
#include <ctime>
#include <unistd.h>
#include "Scheduler.hpp"

using namespace socialine::util::task;

//-------------------------------------------------------------------------
//		Attributes
//-------------------------------------------------------------------------
std::map<time_t, bool> Scheduler::timeoutRoutine = std::map<time_t, bool>();
void (*Scheduler::currentRoutine)();
time_t Scheduler::currentRoutineId;
pthread_t Scheduler::controlThread;


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
bool Scheduler::set_timeout_to_routine(void (*routine)(), long timeout)
{
    run_routine(routine);
    wait_routine_for(timeout);
    finish_routine();

    return !has_routine_finished();
}

void Scheduler::run_routine(void (*function)())
{
    initialize_routine_id();
    
    currentRoutine = function;
    pthread_create(&controlThread, nullptr, control_routine, nullptr);
}

void Scheduler::initialize_routine_id()
{
    currentRoutineId = get_current_time();
    timeoutRoutine[currentRoutineId] = false;
}

time_t Scheduler::get_current_time()
{
    return std::time(nullptr);
}

void* Scheduler::control_routine(void* args)
{
    time_t id = currentRoutineId;
    void (*routine)() = currentRoutine;
    
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
