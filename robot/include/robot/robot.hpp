#pragma once
#include "robot/maze.hpp"
#include "var/var.hpp"

namespace robot {

class Robot {
  public:
    struct robot_stats {
        constexpr const static int MAXIMUM_HAPPINESS = 100;

        // сколько поворотов подряд сделал робот. Параметры истеричности задают
        // границу, выше которой каждый новый поворот снимает очки настроения
        unsigned consequent_rotations = 0;

        // сколько раз подряд у робота спросили об окружении. Параметры
        // истеричности задают границу, выше которой каждый новый вопрос снимает
        // очки настроения
        unsigned consequent_env_asks = 0;

        // Параметр вежливого обращения с роботом. Каждый politely_asked=true вызов
        // ask увеличивает этот показатель, politely_asked=false - уменьшает.
        // Параметры истеричности задают верхнюю и нижнюю границы после которых за
        // вежливость/невежливость начинают сниматься очки настроения
        int pleasure_ask_score = 0;

        // сколько движений вперёд сделал робот. Параметры истеричности задают порог
        // после которого за лишнее движение снимаются очки настроения.
        unsigned movements_count = 0;

        // Общий параметр настроения робота. [0..100]
        // Этот параметр вычисляется исходя из всех внешних факторов и влияет на
        // поведение робота. Чем больше значение, тем меньше вероятность
        // самоуничтожения.
        int happiness_score = MAXIMUM_HAPPINESS;
    };

    struct paranoia_config {
        unsigned punnish_rotations;
        unsigned punnish_env_asks;
        int punnish_politeness_score;
        int punnish_impoliteness_score;
        unsigned punnish_movements;
        int minimal_happiness;
    };

    virtual ~Robot() = default;
    virtual var::var_type get_env(bool debug_call = false) = 0;
    virtual void move() = 0;
    virtual void rotate_r() = 0;
    virtual void rotate_l() = 0;
    virtual void ask(bool is_politely_asked = false) = 0;
    virtual robot_stats get_stats() const noexcept = 0;
    virtual paranoia_config get_paranoia() const noexcept = 0;

    constexpr const static unsigned VIEW_DIAMETER = 11;

  protected:
    static void
    get_env(const Maze &maze, var::var_type &env, unsigned radius) noexcept;
};

} // namespace robot
