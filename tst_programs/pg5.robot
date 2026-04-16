TASK FINDEXIT (
    VAR res = 0
    VAR exe = 1
    VAR A  = TRUE
    VAR B  = FALSE
    SWITCH A TRUE 
    res = res + exe
    SWITCH A OR B TRUE res = res + exe
    FALSE res = exe 
)
