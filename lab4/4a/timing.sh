#./tree.bin > test1

awk 'NR%4==1' test | awk '{print $5}' > search
awk 'NR%4==2' test | awk '{print $5}' > insertion
awk 'NR%4==3' test | awk '{print $5}' > deletion
awk 'NR%4==0' test | awk '{print $5}' > traversal

for i in {1..19}
do 
    awk 'NR%20=='${i} search >> ord_search
    awk 'NR%20=='${i} insertion >> ord_ins
    awk 'NR%20=='${i} deletion >> ord_del
    awk 'NR%20=='${i} traversal >> ord_trav
done 
    awk 'NR%20==0' search >> ord_search
    awk 'NR%20==0' insertion >> ord_ins
    awk 'NR%20==0' deletion >> ord_del
    awk 'NR%20==0' traversal >> ord_trav


echo search
for i in {1..20}
do
    sum_s=0
    sed -n "$((20*i-19)), $((20*i))p" ord_search | awk '{sum_s += $1} END {x=sum_s/20; print x}'
done

echo ins
for i in {1..20}
do
    sum_i=0
    sed -n "$((20*i-19)), $((20*i))p" ord_ins | awk '{sum_i += $1} END {x=sum_i/20; print x}'
done

echo del
for i in {1..20}
do
    sum_d=0
    sed -n "$((20*i-19)), $((20*i))p" ord_del | awk '{sum_d += $1} END {x=sum_d/20; print x}'
done

echo trav
for i in {1..20}
do
    sum_t=0
    sed -n "$((20*i-19)), $((20*i))p" ord_trav | awk '{sum_t += $1} END {x=sum_t/20; print x}'
done
    
    


