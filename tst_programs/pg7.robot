TASK MUL ACCUM, MUL (
    ACCUM = ACCUM * MUL
    RESULT ACCUM
)

TASK CICLE_CHECK C, B, S, ACCUM, MUL (
    FOR C BOUNDARY B STEP S (
    DO MUL ACCUM, MUL
)
RESULT ACCUM
)

TASK FACTORIAL N, RES(
    VAR N1 = 1
    SWITCH ELEQ N FALSE (
        SWITCH ELEQ (N - N1) TRUE (
            RES = 1
        )
        FALSE (
        N = N - N1
        DO FACTORIAL N, RES
        RES = N * RES
        )
)
TRUE RES = 1
RESULT RES
)

TASK FINDEXIT (
    VAR A = 5
    VAR RES = 0
    DO FACTORIAL A, RES
    A = 0
)

