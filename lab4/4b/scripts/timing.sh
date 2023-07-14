#!/bin/bash

############################################
#  data preprocessing 
preproc() 
{
    ./timing.bin > build/test 
    EPOCHS=20
}

############################################
#  select the lines corresponding to each operation
selection() 
{
    awk 'NR%3==1' ../test | awk '{print $5}' > build/search
    awk 'NR%3==2' ../test | awk '{print $5}' > build/insertion
    awk 'NR%3==0' ../test | awk '{print $5}' > build/deletion
    #awk 'NR%4==0' ../test | awk '{print $5}' > build/traversal
}

############################################
# sort lines by the number of nodes in tests
sort ()
{
    #  successfully fill files with nothing
    : > build/ord_search
    : > build/ord_ins
    : > build/ord_del
    : > build/ord_trav

    for i in $(seq 1 $((EPOCHS - 1)))
    do 
        awk 'NR%'$EPOCHS'=='${i} build/search >> build/ord_search
        awk 'NR%'$EPOCHS'=='${i} build/insertion >> build/ord_ins
        awk 'NR%'$EPOCHS'=='${i} build/deletion >> build/ord_del
        #awk 'NR%'$EPOCHS'=='${i} build/traversal >> build/ord_trav
    done 
        awk 'NR%'$EPOCHS'==0' build/search >> build/ord_search
        awk 'NR%'$EPOCHS'==0' build/insertion >> build/ord_ins
        awk 'NR%'$EPOCHS'==0' build/deletion >> build/ord_del
        #awk 'NR%'$EPOCHS'==0' build/traversal >> build/ord_trav
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
        sed -n "$(($EPOCHS*i-$EPOCHS + 1)), $(($EPOCHS*i))p" build/ord_search | awk '{sum_s += $1} END {x=sum_s/'$EPOCHS'*100; print x}'
    done
}
############################################
medium_ins()
{
    echo ins
    for i in $(seq 1 $((EPOCHS)))
    do
        sum_i=0
        sed -n "$(($EPOCHS*i-$EPOCHS + 1)), $(($EPOCHS*i))p" build/ord_ins | awk '{sum_i += $1} END {x=sum_i/'$EPOCHS'*100; print x}'
    done
}
############################################
medium_del()
{
    echo del
    for i in $(seq 1 $((EPOCHS)))
    do
        sum_d=0
        sed -n "$(($EPOCHS*i-$EPOCHS + 1)), $(($EPOCHS*i))p" build/ord_del | awk '{sum_d += $1} END {x=sum_d/'$EPOCHS'*100; print x}'
    done
}
############################################
medium_trav()
{
    echo trav
    for i in $(seq 1 $((EPOCHS)))
    do
        sum_t=0
        sed -n "$(($EPOCHS*i-$EPOCHS + 1)), $(($EPOCHS*i))p" build/ord_trav | awk '{sum_t += $1} END {x=sum_t/'$EPOCHS'; print x}'
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

    
    

