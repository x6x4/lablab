hexdump -Cv table.hex | sed -e '1i table \ ' -e '1a sz kslist_sz \ ' \
-e '2a kslist_max_sz last_node_offset \ ' \
-e '3a erased_nodes_num \ ' -e '4a \ ' \
-e '4a ks1 (num, ks_sz, key, tail) \ ' \
-e '6a ks2 (num, ks_sz, key, tail) \ ' \
-e '8a ks3 (num, ks_sz, key, tail) \ ' -e '10a \ ' \
-e '10a key_node_1, node1 (ver, next, key_offset, val) \ ' \
