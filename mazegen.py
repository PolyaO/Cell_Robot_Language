#!/bin/python3
import random
import sys

WALL = '#'
PATH = ' '
ROBOT = 'R'

EXIT_FACTOR = 50

class Pos:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    @classmethod
    def from_grid_coords(cls, x, y):
        return cls(x*2+1, y*2+1)

    def __add__(self, other):
        return Pos(self.x + other.x, self.y + other.y)

    def __mul__(self, scalar):
        return Pos(self.x * scalar, self.y * scalar)

    def __str__(self):
        return f"pos({self.x},{self.y})"


class Maze:
    def __init__(self, width, height):
        if width % 2 == 0:
            width += 1
        if height % 2 == 0:
            height += 1
        self.width = width
        self.height = height
        self.maze = [[WALL for _ in range(width)] for _ in range(height)]

    def __str__(self):
        # return "\n".join(("".join((a+a for a in row)) for row in self.maze))
        return "\n".join(("".join(row) for row in self.maze))

    def random_pos(self):
        return Pos(
            random.randint(0, self.width // 2 - 1) * 2 + 1,
            random.randint(0, self.height // 2 - 1) * 2 + 1
        )

    def is_valid_pos(self, pos):
        return 0 <= pos.x < self.width and 0 <= pos.y < self.height

    def is_wall(self, pos):
        return self.get_char(pos) == WALL

    def is_path(self, pos):
        return self.get_char(pos) == PATH

    def get_char(self, pos):
        return self.maze[pos.y][pos.x]

    def set_char(self, pos, c):
        self.maze[pos.y][pos.x] = c

    def has_exits(self):
        for i in range(self.width):
            if self.is_path(Pos(i, 0)) or self.is_path(Pos(i, self.height-1)):
                return True
        for i in range(self.height):
            if self.is_path(Pos(0, i)) or self.is_path(Pos(self.width-1, i)):
                return True
        return False

    def carve(self, start_pos):
        self.set_char(start_pos, PATH)

        directions = [Pos(0,1), Pos(1,0), Pos(0,-1), Pos(-1,0)]
        random.shuffle(directions)

        for direction in directions:
            relative_pos = start_pos + (direction * 2)
            if self.is_valid_pos(relative_pos):
                if self.is_wall(relative_pos):
                    self.set_char(start_pos + direction, PATH)
                    self.carve(relative_pos)
            elif random.randint(0, 1000) < EXIT_FACTOR:
                self.set_char(start_pos + direction, PATH)

def get_maze(width, height):
    res = Maze(width, height)
    res.carve(res.random_pos())
    return res

if len(sys.argv) != 3:
    print(f"USAGE: {sys.argv[0]} <width> <height>")
    sys.exit()

while not (maze := get_maze(int(sys.argv[1]), int(sys.argv[2]))).has_exits():
    pass

maze.set_char(maze.random_pos(), ROBOT)

print(str(maze))
