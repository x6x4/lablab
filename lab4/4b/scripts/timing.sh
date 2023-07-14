#!/bin/bash

############################################
#  data preprocessing 
preproc() 
{
    ./timing.bin > bin/test 
    # rm -f bin/ord_del bin/ord_ins bin/ord_search #bin/ord_trav
    EPOCHS=10
}

############################################
#  select the lines corresponding to each operation
selection() 
{
    awk 'NR%3==1' bin/test | awk '{print $5}' > bin/search
    awk 'NR%3==2' bin/test | awk '{print $5}' > bin/insertion
    awk 'NR%3==0' bin/test | awk '{print $5}' > bin/deletion
    # awk 'NR%4==0' bin/test | awk '{print $5}' > bin/traversal
}

############################################
# sort lines by the number of nodes in tests
sort ()
{
    truncate -s 0 bin/ord_del  # clean files before filling
    truncate -s 0 bin/ord_ins
    truncate -s 0 bin/ord_search
    truncate -s 0 bin/ord_trav

    for i in $(seq 1 $((EPOCHS - 1)))
    do 
        awk 'NR%'$EPOCHS'=='${i} bin/search    >> bin/ord_search
        awk 'NR%'$EPOCHS'=='${i} bin/insertion >> bin/ord_ins
        awk 'NR%'$EPOCHS'=='${i} bin/deletion  >> bin/ord_del
        # awk 'NR%'$EPOCHS'=='${i} bin/traversal >> bin/ord_trav
    done 
        awk 'NR%'$EPOCHS'==0' bin/search    >> bin/ord_search
        awk 'NR%'$EPOCHS'==0' bin/insertion >> bin/ord_ins
        awk 'NR%'$EPOCHS'==0' bin/deletion  >> bin/ord_del
        # awk 'NR%'$EPOCHS'==0' bin/traversal >> bin/ord_trav
}

############################################
#  calculate medium for each operation
medium()
{
    medium_search
    medium_ins
    medium_del
    #medium_trav
}
############################################
medium_search() 
{
    echo search
    for i in $(seq 1 $((EPOCHS)))
    do
        sum_s=0
        sed -n "$(($EPOCHS*i-$EPOCHS + 1)), $(($EPOCHS*i))p" bin/ord_search | awk '{sum_s += $1} END {x=sum_s/'$EPOCHS'*100; print x}'
    done
}
############################################
medium_ins()
{
    echo ins
    for i in $(seq 1 $((EPOCHS)))
    do
        sum_i=0
        sed -n "$(($EPOCHS*i-$EPOCHS + 1)), $(($EPOCHS*i))p" bin/ord_ins | awk '{sum_i += $1} END {x=sum_i/'$EPOCHS'*100; print x}'
    done
}
############################################
medium_del()
{
    echo del
    for i in $(seq 1 $((EPOCHS)))
    do
        sum_d=0
        sed -n "$(($EPOCHS*i-$EPOCHS + 1)), $(($EPOCHS*i))p" bin/ord_del | awk '{sum_d += $1} END {x=sum_d/'$EPOCHS'*100; print x}'
    done
}
############################################
medium_trav()
{
    echo trav
    for i in $(seq 1 $((EPOCHS)))
    do
        sum_t=0
        sed -n "$(($EPOCHS*i-$EPOCHS + 1)), $(($EPOCHS*i))p" bin/ord_trav | awk '{sum_t += $1} END {x=sum_t/'$EPOCHS'; print x}'
    done
}

############################################
main()
{
	preproc
    selection
    sort
    medium
}
############################################
main

    
    

