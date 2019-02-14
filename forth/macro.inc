    %define last 0

    %macro native 3
    section.data
w_%+%2: 
    dq last
    %define last w_%+%2
    db %1, 0
    db %3
xt_%+%2:    
    dq %2%+_impl
    section .text
%2%+_impl:
    %endmacro

    %macro native 2
    native %1, %2, 0
    %endmacro

    %macro colon 3
    section .data
w_%+%2:
    dq last
    %define last w_%+%2
    db %1, 0
    db %3
xt_%+%2:
    dq docol_impl
    %endmacro

    %macro colon 2
    colon %1, %2, 0
    %endmacro
