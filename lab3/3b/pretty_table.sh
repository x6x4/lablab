hexdump -v table.hex | sed -e '1i table \ ' -e '1a sz kslist_sz \ ' \
-e '2a kslist_max_sz last_node_offset \ ' -e '3a \ ' \
-e '3a ks1 (num, ks_sz, key, tail) \ ' \
-e '7a ks2 (num, ks_sz, key, tail) \ ' \
-e '11a ks3 (num, ks_sz, key, tail) \ ' -e '15a \ ' \
-e '15a node1 (ver, next, key_offset, val) \ ' \
-e '17a node2 \ ' -e '19a node3 \ ' -e '21a node4 \ ' \
-e '23a node5 \ ' -e '25a node6 \ ' -e '27a node7 \ ' \
-e '29a node8 \ ' -e '31a node9 \ ' -e '33a node10 \ ' \
