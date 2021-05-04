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


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
bool Scheduler::set_timeout_to_routine(void (*routine)(), int timeout)
{
    time_t id = get_current_time();
    
    pthread_t controlThread = run_routine(routine, id);
    wait_for(timeout);
    finish_routine(controlThread);

    return !timeoutRoutine[id];
}

pthread_t Scheduler::run_routine(void (*function)(), time_t id)
{
    pthread_t controlThread;

    currentRoutine = function;
    currentRoutineId = id;
    pthread_create(&controlThread, nullptr, control_routine, nullptr);

    return controlThread;
}

void Scheduler::finish_routine(pthread_t routineThread)
{
    pthread_cancel(routineThread);
}

void* Scheduler::control_routine(void* args)
{
    time_t id = currentRoutineId;
    void (*routine)() = currentRoutine;
    
    timeoutRoutine[id] = false;
    routine();
    timeoutRoutine[id] = true;
}

void Scheduler::wait_for(int time)
{
    time_t id = currentRoutineId;
    time_t start = get_current_time();

    while ((difftime(get_current_time(), start)*1000 < (double)time) && !timeoutRoutine[id])
        sleep(1);
}

time_t Scheduler::get_current_time()
{
    return std::time(nullptr);
}
