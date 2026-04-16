TASK SUM VAR1, VAR2, VAR3 (
    VAR3 = VAR2 + VAR1
    RESULT VAR3
)

TASK ASD RES (
    PLEASE VAR C = 0 THANKS
    VAR B = 10
    VAR S = 2
    VAR ASD [2,2,3] = 1
    VAR D  [2,2,3] = 2
    FOR C BOUNDARY B STEP S (
        ASD = ASD * D
    )
    RES = ASD
    RESULT RES
)

TASK FINDEXIT (
    VAR A [1,2,3] = 1
    VAR B [1,2,3] = 2
    VAR C [1,2,3] = 0
    VAR R [2,2,3] = 0
    DO SUM A, B, C
)
