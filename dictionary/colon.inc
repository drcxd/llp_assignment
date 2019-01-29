    %define last 0

    %macro colon 2              ; %1 key %2 label name
%%this: 
    dq last
    %define last %%this
    db %1, 0
    %endmacro
