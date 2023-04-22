/**
 * @file DemobotConfig.hpp
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Specifies target IP for each robot type.
 * @version 0.0.0
 * @date 2023-04-20
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include <IPAddress.h>

/** List of all possible Demobots the DemobotNetwork supports. */
enum DemobotID {
    DANCEBOT,
    POLARGRAPH,
    MARQUEE,
    TOWER_OF_POWER
};

/** Gets the robot IP given the id. */
IPAddress get_robot_ip(enum DemobotID id);