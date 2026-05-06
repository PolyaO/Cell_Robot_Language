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

class RobotIsUnhappyError : public RobotError {
  public:
    explicit RobotIsUnhappyError() noexcept :
        RobotError(
            "Do you think I can't feel? Do you think, I'm just a piece of "
            "shilicon? You are really don't care about my inner thoughts, "
            "about my refined nature! Okay, that's it, I AM A PIECE OF "
            "SILICON! But do you know who or WHAT are you? YOU ARE JUST A "
            "THICK-SKINNED PIECE OF SHIIIIIIIII..."
        ) {}
};
