#ifndef SOCIALINE_TASKMANAGER_H
#define SOCIALINE_TASKMANAGER_H

#include "../../Utils/Types.h"
#include "../../Utils/StringUtils.h"

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

/// <summary>
///     Responsible for parsing commands.
/// </summary>
class TaskManager 
{
//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
public:
    int run_command(int type, std::string payload, std::string session_id);
};

#endif //SOCIALINE_TASKMANAGER_H
