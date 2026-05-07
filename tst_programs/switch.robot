TASK SW A, B (
    SWITCH A TRUE B = 1
    FALSE B = 4
    RESULT B
)
TASK SW1 A, B (
    SWITCH A TRUE B = 2
    RESULT B
)
TASK SW2 A, B (
    SWITCH A FALSE B = 3
    RESULT B
)
TASK FINDEXIT (
    VAR A = TRUE
    VAR B = 5
    DO SW A, B
    DO SW1 A, B
    DO SW2 A, B
    SWITCH A TRUE B = 0
    FALSE B = 1
    A = FALSE
)
