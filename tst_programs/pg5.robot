TASK CHECKSWITCH V, RES(
    VAR one = 1
    VAR zero = 0
    SWITCH MXTRUE V
    TRUE (
        RES = one
    )
    FALSE (
        RES = zero
    )
    RESULT RES
)
TASK FINDEXIT (
    VAR r = 5
    VAR A [1,2,3] = TRUE
    VAR b [3] = FALSE
    A [1] = b
    DO CHECKSWITCH A, r
    VAR D = 0
)
