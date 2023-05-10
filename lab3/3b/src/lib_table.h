#include <stdlib.h>

#define KSLIST_MAX_SZ 3
#define ERASED_NODES_THRESHOLD 5

/*  To init last_node_offset in table creation.  */
#define ks_offset(i) TABLE+KS*(i-1)
/*  For new node creation.  */
#define ks_key_offset(i) ks_offset(i) + 2*sizeof (size_t)

/*  0x10 bytes - format + filename, 0x28 - 5 fields of size 8  */
#define TABLE 0x40 
#define KS (sizeof (ks_d))
#define NODE (sizeof (node_d))

typedef struct table_ram table_ram;
typedef struct keyspace ks_d;
typedef struct node_d node_d;

typedef size_t offset_t;

/*  Table descriptor in ram.  */
struct table_ram {
    size_t sz;
    size_t kslist_sz;
    size_t kslist_max_sz;
    offset_t last_node_offset;
    size_t erased_nodes_num;

    ks_d *kslist;
};

/*  Keyspace descriptor.  */
struct keyspace {
    size_t num_in_table;
    size_t ks_sz;
    offset_t key;
    offset_t tail;
};

/*  Linked list node with info.  */
struct node_d
{
    size_t ver;
    offset_t next;
    offset_t key;
    size_t val;
};

#define KEY_OFFSET_IN_NODE (sizeof(node_d) - sizeof(size_t) - sizeof(offset_t))