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
    VAR C1 = 1
    VAR C2 = 2
    VAR C3 = 3
    VAR C4 = 4
    VAR C5 = 5
    VAR C6 = 6
    VAR S1 = 1
    VAR B10 = 10
    C [1, 1] = C1
    C [1, 2] = C2
    C [1, 3] = C3
    C [2, 1] = C4
    C [2, 2] = C5
    C [2, 3] = C6
    VAR B [2,3] = 0
    B [2, 1] = B10
    B [2, 2] = B10
    B [2, 3] = B10
    VAR S [2,3] = -1
    S [2, 1] = C2
    S [2, 2] = C2
    S [2, 3] = C2
    DO REFFOR ACCUM, C, B, S
    VAR ASD [2,2,3] = 0
)
