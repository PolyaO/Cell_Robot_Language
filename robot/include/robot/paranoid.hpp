#pragma once

#include "robot/maze.hpp"
#include "robot/robot.hpp"
#include "var/bool.hpp"
#include "var/var.hpp"
#include <random>

namespace robot {

struct paranoia_config {
    unsigned punnish_rotations;
    unsigned punnish_env_asks;
    int punnish_politeness_score;
    int punnish_impoliteness_score;
    unsigned punnish_movements;
    int minimal_happiness;
};

class ParanoiaConfigGenerator {
  public:
    ParanoiaConfigGenerator(int seed) : gen(seed) {}

    // стандартные границы распределения количества ненаказуемых
    // последовательных поворотов
    constexpr const static unsigned PUNNISH_ROTATIONS_MIN = 5;
    constexpr const static unsigned PUNNISH_ROTATIONS_MAX = 20;
    // стандартные границы распределения количества ненаказуемых
    // последовательных запросов окружения
    constexpr const static unsigned PUNNISH_ENV_ASKS_MIN = 5;
    constexpr const static unsigned PUNNISH_ENV_ASKS_MAX = 10;

    // стандартные границы распределения показателя вежливости
    constexpr const static int PUNNISH_POLITENESS_SCORE_MIN = 15;
    constexpr const static int PUNNISH_POLITENESS_SCORE_MAX = 30;

    // стандартные границы распределения показателя невежливости
    constexpr const static int PUNNISH_IMPOLITENESS_SCORE_MAX = -15;
    constexpr const static int PUNNISH_IMPOLITENESS_SCORE_MIN = -30;

    // стандартные границы распределения количества ненаказуемых движений вперёд
    constexpr const static unsigned PUNNISH_MOVEMENTS_MIN = 50;
    constexpr const static unsigned PUNNISH_MOVEMENTS_MAX = 200;

    // стандартные границы распределения минимального несамоубийственного
    // счастья
    constexpr const static int MINIMAL_HAPPINESS_MIN = 0;
    constexpr const static int MINIMAL_HAPPINESS_MAX = 15;

    paranoia_config random_paranoia_scores() noexcept;

  private:
    std::mt19937 gen;
    static inline std::uniform_int_distribution<unsigned> rotations_dist{
        PUNNISH_ROTATIONS_MIN, PUNNISH_ROTATIONS_MAX
    };

    static inline std::uniform_int_distribution<unsigned> env_asks_dist{
        PUNNISH_ENV_ASKS_MIN, PUNNISH_ENV_ASKS_MAX
    };

    static inline std::uniform_int_distribution<int> politeness_score_dist{
        PUNNISH_POLITENESS_SCORE_MIN, PUNNISH_POLITENESS_SCORE_MAX
    };

    static inline std::uniform_int_distribution<int> impoliteness_score_dist{
        PUNNISH_IMPOLITENESS_SCORE_MIN, PUNNISH_IMPOLITENESS_SCORE_MAX
    };

    static inline std::uniform_int_distribution<unsigned> movements_dist{
        PUNNISH_MOVEMENTS_MIN, PUNNISH_MOVEMENTS_MAX
    };

    static inline std::uniform_int_distribution<int> minimal_happiness_dist{
        MINIMAL_HAPPINESS_MIN, MINIMAL_HAPPINESS_MAX
    };
};

class Paranoid : public Robot {
  public:
    Paranoid(Maze &maze, const paranoia_config &cfg);
    Paranoid(Maze &maze, ParanoiaConfigGenerator &);
    var::var_type get_env(bool debug_call = false) override;
    void move() override;
    void rotate_r() override;
    void rotate_l() override;
    void ask(bool is_politely_asked = false) override;

  private:
    void update_happiness();
    void update_current_env();

    Maze &maze;
    paranoia_config cfg;
    var::var_type env = var::Var<bool_t>(false, {11, 11, 2});

    constexpr const static int MAXIMUM_HAPPINESS = 100;
    constexpr const static int STAY_TOO_LONG_PUNNISH = 1;
    constexpr const static int VIEW_EXIT_HAPPINESS = 2;

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

} // namespace robot
