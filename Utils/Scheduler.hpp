#pragma once
#include <functional>
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
        static std::function<void(void)>& currentRoutine;
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
        /// <summary>
        ///     Repeatedly calls a routine with a fixed time delay between each
        ///     call.
        /// </summary>
        ///
        /// <param name="routine">Routine to be performed</param>
        /// <param name="interval">
        ///     Interval that the routine will be invoked (in milliseconds)
        /// </param>
        ///
        /// <returns>
        ///     Routine id, being useful for future cancellation
        /// </returns>
        ///
        /// <seealso cref="Scheduler::clear_interval"/>
        static long set_interval(const std::function<void(void)>& routine, long interval);

        /// <summary>
        ///     Cancels a timed, repeating action, which was previously established by a
        ///     call to <see cref="Scheduler::set_interval"/>.
        /// </summary>
        ///
        /// <param name="id">Routine id</param>
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
        static bool set_timeout_to_routine(const std::function<void(void)>& routine, long timeout);
    private:
        static void run_routine(const std::function<void(void)>& routine);
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
