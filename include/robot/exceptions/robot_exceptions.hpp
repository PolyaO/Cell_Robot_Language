#pragma once

#include <stdexcept>

class RobotError : public std::runtime_error {
  public:
    explicit RobotError(const std::string &msg) noexcept :
        std::runtime_error("Robot panic: " + msg) {}
};

class RobotMoveIntoTheWallError : public RobotError {
  public:
    explicit RobotMoveIntoTheWallError() noexcept :
        RobotError("Do you think, I'm silly? Therre is the wall herrre! :(") {}
};
