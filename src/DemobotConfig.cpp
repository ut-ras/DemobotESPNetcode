/**
 * @file DemobotConfig.cpp
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Specifies target IP for each robot type.
 * @version 0.0.0
 * @date 2023-04-20
 * @copyright Copyright (c) 2023
 *
 */
#include "DemobotConfig.hpp"

IPAddress get_robot_ip(enum DemobotID id) {
    /* Set server ip based on robot type. */
    switch(id) {
        case DANCEBOT:
            return IPAddress(192,168,2,1);
        case POLARGRAPH:
            return IPAddress(192,168,2,2);
        case MARQUEE:
            return IPAddress(192,168,2,3);
        case TOWER_OF_POWER:
            return IPAddress(192,168,2,4);
        default:
            return IPAddress(192,168,2,0);
    }
}

