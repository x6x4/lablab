hexdump -v table.bin | sed -e '1i table \ ' -e '1a sz kslist_sz \ ' \
-e '2a kslist_max_sz last_node_offset \ ' -e '3a \ ' \
-e '3a ks1 (num, ks_sz, key, tail) \ ' \
-e '7a ks2 (num, ks_sz, key, tail) \ ' -e '11a \ ' \
-e '11a node1 (ver, next, key_offset, val) \ ' \
-e '13a node2 \ ' -e '15a node3 \ ' -e '17a node4 \ ' \
-e '19a node5 \ ' -e '21a node6 \ ' -e '23a node7 \ ' \
