TASK SUM VAR1, VAR2, VAR3 (
    VAR B [1,2,3] = 5
    VAR3 = VAR2 + VAR1
    RESULT VAR3
)

TASK ASD RES (
    PLEASE VAR C = 5 THANKS
    VAR B = 10
    VAR S = 2
    VAR ASD [2,2,3] = 1
    VAR D  [2,2,3] = 2
    FOR C BOUNDARY B STEP S
 ASD = ASD * D
    RES = ASD
    RESULT RES
)

TASK REFFOR RES, C, B, S (
    VAR ASD = 1
    FOR C BOUNDARY B STEP S RES = RES + ASD
    ASD = ASD
    RESULT RES
)

TASK FINDEXIT (
    VAR ACCUM = 0
    VAR C [2,3] = 0
    C [1, 1] = 1
    C [1, 2] = 2
    C [1, 3] = 3
    C [2, 1] = 4
    C [2, 2] = 5
    C [2, 3] = 6
    VAR B [2,3] = 0
    B [2] = 10
    VAR S [2,3] = -1
    S [2] = 2
    DO REFFOR ACCUM, C, B, S
    VAR ASD [2,2,3] = 0
)
