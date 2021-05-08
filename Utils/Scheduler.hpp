#pragma once
#include <pthread.h>
#include <map>

namespace socialine::util::task
{
    /// <summary>
    ///     Responsible for executing routines within time intervals.
    /// </summary
    class Scheduler
    {
    //-------------------------------------------------------------------------
    //		Attributes
    //-------------------------------------------------------------------------
    private:
        static std::map<long, bool> intervalRoutines;
        static std::map<long, bool> timeoutRoutine;
        static long currentRoutineId;
        static void (*currentRoutine)();
        static pthread_t controlThread;


    //-------------------------------------------------------------------------
    //		Constructor
    //-------------------------------------------------------------------------
    private:
        Scheduler();


    //-------------------------------------------------------------------------
    //		Methods
    //-------------------------------------------------------------------------
    public:
        static long set_interval(void (*routine)(), long interval);
        static void clear_interval(long id);

        /// <summary>
        ///     Runs a routine within a timeout.
        /// </summary>
        ///
        /// <param name="function">Routine</param>
        /// <param name="timeout">Maximum execution time (in milliseconds)</param>
        ///
        /// <returns>
        ///     True if the routine has not finished executing within the time
        ///     limit; false otherwise
        /// </returns>
        static bool set_timeout_to_routine(void (*routine)(), long timeout);
    private:
        static void run_routine(void (*routine)());
        static void initialize_routine_id();
        static time_t get_current_time();
        static void* interval_control_routine(void* args);
        static void* control_routine(void* args);
        static void wait_routine_for(long time);
        static double time_elapsed_in_milliseconds(time_t start);
        static bool has_routine_finished();
        static void finish_routine();
    };
}
