#pragma once

#include <format>
#include <stdexcept>

class MazeError : public std::runtime_error {
  public:
    explicit MazeError(const std::string &msg) noexcept :
        std::runtime_error(msg) {}
};

class MazeLoadError : public MazeError {
  public:
    explicit MazeLoadError(const std::string &msg) noexcept :
        MazeError("Error loading maze: " + msg) {}
};

class MazeLoadNewLineLengthError : public MazeLoadError {
  public:
    explicit MazeLoadNewLineLengthError(
        int line_no, int new_length, int old_length
    ) noexcept :
        MazeLoadError(
            std::format(
                "at line {}: length is {} -- differs from lengths of previous "
                "lines {}",
                line_no,
                new_length,
                old_length
            )
        ) {}
};

class MazeLoadInvalidWidthError : public MazeLoadError {
  public:
    explicit MazeLoadInvalidWidthError(int width, int at_least) noexcept :
        MazeLoadError(
            std::format(
                "width of maze is {} which is too short -- needs at least {}",
                width,
                at_least
            )
        ) {}
};

class MazeLoadInvalidHeightError : public MazeLoadError {
  public:
    explicit MazeLoadInvalidHeightError(int height, int at_least) noexcept :
        MazeLoadError(
            std::format(
                "height of maze is {} which is too short -- needs at least {}",
                height,
                at_least
            )
        ) {}
};
