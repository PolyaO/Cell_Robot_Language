TASK FACTORIAL N, RES(
    VAR N1 = 1
    VAR LOCRES = 0
    SWITCH ELEQ N FALSE (
        SWITCH ELEQ (N - N1) TRUE (
            RES = 1
        )
        FALSE (
        VAR N_1 = 0
        N_1 = N - N1
        DO FACTORIAL N_1, LOCRES
        RES = N * LOCRES
        )
)
TRUE RES = 1
RESULT RES
)

TASK FINDEXIT (
    VAR A = 5
    VAR RES = 0
    DO FACTORIAL A, RES
    A = RES
    A = 0
)

