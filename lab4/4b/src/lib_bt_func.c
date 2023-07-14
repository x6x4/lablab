#include "lib_bt_func.h"
#include "generic.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NO_3VERTEX CHILD_NUM - 1

/*  TREE  */

/*  Insertion  */ 

/*  
    insert in list if duplicate key;
    calls insert_to_tree otherwise 
*/
int insert_bt (BNodePtr *root, Key key, char *val) {
    size_t pos = 0;
    BNodePtr node = find_bt (*root, key, &pos);

    InfoListPtr info = !node ? NULL : node->info[pos];

    if (branch_ext (&info, key, val) == ERRDUP) 
        return ERRDUP;

    info->csize++;
    
    /*  branch ext  */
    if (node)
        return ERRSUC;

    /*  standard insertion  */
    insert_to_tree (root, *root, info);
    
    return ERRSUC;
}

/*  insertion is always performed to the leaf  */
void insert_to_tree (BNodePtr *root, BNodePtr cnode, InfoListPtr info) {

    /*  create root  */
    if (!(*root)) {
        *root = new_vertex (info);
        return;
    }
 
    /*  insert to leaf  */
    if (is_leaf (cnode)) {
        insert_to_vertex (cnode, info);
        split_node (root, cnode);
        return;
    }

    /*  recursive descent  */
    for (size_t i = 0; i < cnode->csize; i++) {
        
        /*  compare from the beginning of the node until meeting a greater key  */
        if ( GT (cnode->info[i]->key, info->key)) {
            insert_to_tree (root, cnode->child[i], info);
            return;
        }
    }

    /*  if key is greater than all keys in the node  */
    insert_to_tree (root, cnode->child[cnode->csize], info);
}
    
/*  recursive pushing  */    
void split_node (BNodePtr *root, BNodePtr node) {
    
    if (node->csize < 3)
        return;

    /*  vertex with 3 keys  */

    BNodePtr left = NULL, right = NULL;
    split_middle (node, &left, &right);
    
    /*  end case - create new root  */
    if (node == *root) {
        left->par = *root;
        right->par = *root;
        construct_root_after_split (*root, (*root)->info[1], left, right);
        return;
    }

    /*  push middle to par  */
    else {
        insert_to_vertex (node->par, node->info[1]);
        
        /*  clear split node field  */
        for (size_t i = 0; i < CHILD_NUM - 1; i++) {
            if (node->par->child[i] == node) {
                node->par->child[i] = NULL;
                break;
            }
        }
        
        for (size_t i = 0; i < 3; i++) {
            if (!(node->par->child[i])) {
                /*  move to make room for two new children  */
                node->par->child[3] = node->par->child[2];
                node->par->child[2] = node->par->child[1];

                /*  fill released space with left and right  */
                node->par->child[i] = left;
                node->par->child[i+1] = right;    
                break;
            }
        }

        BNodePtr par = node->par;
        free_nullify (node);

        /*  now split filled parent  */
        split_node (root, par);
    }
}

/*  move down edge keys and make them children of the middle key  */
void split_middle (BNodePtr node, BNodePtr *left, BNodePtr *right) {

    BNodePtr left_children [4] = {node->child[0], node->child[1], NULL, NULL};
    *left = new_bt_node (node->info[0], left_children, node->par);
    
    BNodePtr right_children [4] = {node->child[2], node->child[3], NULL, NULL};
    *right = new_bt_node (node->info[2], right_children, node->par);

    /*  set left and right as parents  */
    for (size_t i = 0; i < 2; i++) {
        if ((*left)->child[i])
            (*left)->child[i]->par = *left;
        if ((*right)->child[i])
            (*right)->child[i]->par = *right;
    }
}


/*  Print.  */

void colored_print_bt (BNodePtr root, char *key) {
    set_height (root);
    colored_print_bt_lvl (root, 0, key);
}

void set_height (BNodePtr root) {
    if (!root)
        return;

    /*  Set children height  */    
    for (size_t i = 0; i < CHILD_NUM - 1; i++) {
        if (root->child[i])
            root->child[i]->height = root->height + 1;
    }
    /*  Recursion.  */
    for (size_t i = 0; i < CHILD_NUM - 1; i++) 
        set_height (root->child[i]);
} 

void colored_print_bt_lvl (BNodePtr root, size_t height, char *key) {
    static size_t no_children = 0;

    /*  Print null child if there are brothers.  */
    if (!root) {
        if (!no_children)
            printf ("\n%*s└── ", (int) (4*(height+1) - 4), ""); 
        return;
    }

    /*  Set height.  */
    if (root->height != height) {
        printf ("\n");
        height = root->height;
    }

    /*  Depict non-root node.  */
    if (height) 
        printf ("%*s└── ", (int) (4*height - 4), ""); 

    colored_print_node (root, key);

    /*  Print all the children.  */
    for (size_t i = 0; i < CHILD_NUM - 1; i++) {
        no_children = 1;
        for (size_t j = 0; j < CHILD_NUM - 1; j++) {
            if (root->child[j])
                no_children = 0;
        }
        colored_print_bt_lvl (root->child[i], height, key);
    }
}

void colored_print_node (BNodePtr root, Key key) {
    /*  Color desired key.  */
    for (size_t i = 0; i < root->csize; i++) {
        if (key && EQ(root->info[i]->key, key)) {
            printf (RED("%s(%lu): "), root->info[i]->key, root->info[i]->csize);
            print_ll (root->info[i]->head);
        }
        else 
            printf ("%s(%lu) ", root->info[i]->key, root->info[i]->csize);
    }
}

/*  Search  */

/*  
    returns NULL if key not found; 
    the node and the position of the key in it otherwise
*/
BNodePtr find_bt (BNodePtr root, Key key, size_t *pos) {
    
    /*  the bad end  */
    if (!root) 
        return NULL;

    /*  the good end  */
    if (find_in_vertex (root, key, pos) == ERRSUC) 
        return root;

    int for_descent = chld_for_descent (root, key);
    return find_bt (root->child[for_descent], key, pos);
}

int chld_for_descent (BNodePtr root, Key key) {
    /*  key < chld0 (< chld1)  */
    if (LT(key, root->info[0]->key))
        return 0;

    /*  chld0 < key < (chld1)  */
    if (root->csize == 2 && LT(key, root->info[1]->key) 
        || root->csize == 1)
        return 1;

    /*  chld0 < chld1 < key  */
    if (root->csize == 2)
        return 2;

    return -1;
}

BNodePtr find_max_node (BNodePtr root) {
    if (root == NULL || root->child[0] == NULL /*leaf*/ )
        return root;

    /*  max_node - greatest non-null child of the node  */
    for (size_t i = 0; i < CHILD_NUM - 1; i++) {
        if (root->child[i+1] == NULL) 
            return find_max_node (root->child[i]);
    }
    return find_max_node (root->child[CHILD_NUM-1]);
}

BNodePtr find_min_node (BNodePtr root) {
    if (root == NULL || root->child[0] == NULL /*leaf*/ )
        return root;

    /*  min_node - 1st child of the node  */
    return find_max_node (root->child[0]);
}


/*  Deletion.  */

/*  deletion is always performed from the leaf  */
int delete_bt (BNodePtr *root, Key key, size_t ver) {

    /*  Deletion from infolist.  */

    size_t key_pos = 0;
   
    BNodePtr victim = find_bt (*root, key, &key_pos);
    if (!victim)
        return ERRWRG;

    int status = delete_list_node (key, ver, victim, key_pos);

    if (status != ERRCONT)
        return status;

    /*  Deletion from tree  */

    /*  
        if node has no left child, it is already a leaf;
        otherwise, we switch to the rightmost node of greatest less subtree
        and move its value to the place of the victim key  
    */

    InfoListPtr *exchange_key = NULL;
    BNodePtr exchange_leaf = find_max_node (victim->child[key_pos]);
    InfoListPtr *key_place = &(victim->info[key_pos]);
    
    if (exchange_leaf) {
        exchange_key = &(exchange_leaf->info[exchange_leaf->csize - 1]);
        swap (exchange_key, key_place);
        victim = exchange_leaf;
    }

    /*  delete key in leaf  */
    if (exchange_key)
        free_infolist (exchange_key);
    else 
        free_infolist (key_place);

    /*  fill in the freed node cell.  */
    shift_infolists_and_change_sz (victim, key);

    /*  fix bt  */
    fix_after_del (root, victim);

    return ERRSUC;
}

int delete_list_node (Key key, size_t ver, BNodePtr victim, size_t key_pos) {

    InfoPtr *head = &(victim->info[key_pos]->head);

    if (delete_from_ll (head, ver) == ERRWRG)
        return ERRWRG;

    victim->info[key_pos]->csize--;

    if (*head)
        return ERRSUC;
    else 
        return ERRCONT;
}

void fix_after_del (BNodePtr *root, BNodePtr leaf) {

    if (!leaf)
        return;

    BNodePtr par = leaf->par;

    if (leaf->csize != 0) {
        /*  leaf is non-empty */
        if (par)
            //  ?????
            fix_after_del (root, par);
        else 
            //  root
        return;
    } 
    /*  the only key in the tree  */
    else if (leaf->csize == 0 && par == NULL) {
        //  delete root
        free_nullify(*root);
        return;
    } 
    else {
        /*  emptied leaf  */

        size_t v3_chld = NO_3VERTEX;
        v3_chld = get_v3_num (par, leaf);

        if (NO_3VERTEX == v3_chld && par->csize < 2)
            //  1 node in parent and all children
            leaf = merge (root, leaf);
        else 
            leaf = redistribute (leaf);
        
        /*  recursion  */
        fix_after_del (root, leaf);
    }
}

/*  merge - par of size 1 has two chidren of size 1  */
BNodePtr merge (BNodePtr *root, BNodePtr leaf) {
    BNodePtr par = leaf->par;
    if (!par)
        return NULL;

    size_t victim_num = CHILD_NUM - 2;

    if (leaf == par->child[0]) 
        victim_num = 0;
    else if (leaf == par->child[1]) 
        victim_num = 1;

    move_par_key_to_nonnull_chld (victim_num, par);
    
    /*  
        there are always 3 of them
        because "leaf" can't have more than 1 child
        (due to ascending manner of deletion)
    */
    assign_grandchildren_to_nonnull_chld (victim_num, par, leaf);
    clear_par_and_leaf_ (par, victim_num);
    
    if (par == *root) {
        //  make new root
        BNodePtr new_root = get_nonnull_child (par);
        new_root->par = NULL;
        *root = new_root;

        free_vertex (&par);
        
        return new_root;
    } else 
        return par;
}

void move_par_key_to_nonnull_chld (size_t victim_num, BNodePtr par) {
    switch (victim_num) {
        case 0:
            insert_to_vertex (par->child[1], par->info[0]);
            break;
        case 1:
            insert_to_vertex (par->child[0], par->info[0]);
    }
}

void assign_grandchildren_to_nonnull_chld (size_t victim_num, BNodePtr par, BNodePtr leaf) {
    switch (victim_num) {
        case 0:
            par->child[1]->child[2] = par->child[1]->child[1];
            par->child[1]->child[1] = par->child[1]->child[0];
            par->child[1]->child[0] = get_nonnull_child (leaf);
            if (par->child[1]->child[0])
                par->child[1]->child[0]->par = par->child[1];
            break;
        case 1:
            par->child[0]->child[2] = get_nonnull_child (leaf);
            if (par->child[0]->child[2])
                par->child[0]->child[2]->par = par->child[0];
    }
}

void clear_par_and_leaf_ (BNodePtr par, size_t victim_num) {
    shift_infolists_and_change_sz (par, par->info[0]->key);
    // when empty "leaf" was kept non-null because of non-null child
    free_vertex (&(par->child[victim_num]));
}

size_t get_v3_num (BNodePtr par, BNodePtr leaf) {

    for (size_t i = 0; i < par->csize + 1; i++) {
        if (par->child[i]->csize == 2)
            return i;
    }
    return NO_3VERTEX;
}

/*  redistribute  */
BNodePtr redistribute (BNodePtr leaf) {

    BNodePtr par = leaf->par;
    if (!par)
        return leaf;

    size_t leaf_num = set_leaf_num (par, leaf);
    size_t vertex3_num = get_v3_num (par, leaf);

    if (par->csize == 2 && NO_3VERTEX == vertex3_num) {
        lshift_par_children (leaf_num, par);
        insert_parent_key_to_chld (leaf_num, par);

        /*  right shift  */
        if (leaf_num == 0) {
            for (size_t i = CHILD_NUM - 2; i > 0; i--)
                par->child[0]->child[i] = par->child[0]->child[i-1];
        }

        //  "leaf" can't have more than 1 child
        //  (due to ascending order of deletion)
        insert_missing_child (leaf_num, par, leaf);

        clear_par_and_leaf (leaf_num, par, leaf);
    } 

    else if (par->csize == 2) {
        
        if (leaf_num == 0) {
            //  leaf can't be 3-vertex, because it's empty
            if (vertex3_num == 1) 
                rotate_left (1, 0, par);
            else if (vertex3_num == 2) {
                rotate_left (1, 0, par);  
                rotate_left (2, 1, par);
            }
        }
        else if (leaf_num == 1) {
            if (vertex3_num == 2) 
                rotate_left (2, 1, par);
            else if (vertex3_num == 0) 
                rotate_right (0, 1, par);  
        }
        else if (leaf_num == 2) {
            if (vertex3_num == 1) 
                rotate_right (1, 2, par);
            else if (vertex3_num == 2) {
                rotate_right (1, 2, par);
                par->child[1]->child[1] = par->child[1]->child[0];
                rotate_right (0, 1, par);
            } 
        }
    }   
       
    else if (par->csize == 1) {
        if (leaf_num == 0 && vertex3_num == 1) {
            /*  left shift  */
            if (leaf->child[0] == NULL) 
                leaf->child[0] = leaf->child[1];

            rotate_left (1, 0, par);
        } 
        else if (leaf_num == 1 && vertex3_num == 0) {
            /*  right shift  */
            if (leaf->child[1] == NULL) 
                leaf->child[1] = leaf->child[0];

            rotate_right (0, 1, par);
        }
    }

    /*  ascend  */
    return par;
}

void lshift_par_children (size_t victim_num, BNodePtr par) {
    switch (victim_num) {
        case 0:
            par->child[0] = par->child[1];
        case 1:
            par->child[1] = par->child[2];
        case 2:
            par->child[2] = NULL;
    }
}

void insert_parent_key_to_chld (size_t victim_num, BNodePtr par) {
    switch (victim_num) {
        case 0:
        case 1:
            insert_to_vertex (par->child[0], par->info[0]);
            break;
        case 2:
            insert_to_vertex (par->child[1], par->info[1]);
    }
}

void insert_missing_child (size_t victim_num, BNodePtr par, BNodePtr leaf) {
    BNodePtr node_to_insert = NULL;
    BNodePtr possible_par = NULL;

    for (size_t i = 0; i < CHILD_NUM - 2; i++) {
        if (leaf->child[i]) {
            node_to_insert = leaf->child[i];
            break;
        }
    }
    
    BNodePtr *place_to_insert = NULL;

    switch (victim_num) {
        case 0:
            place_to_insert = &(par->child[0]->child[0]);
            possible_par = par->child[0];
            break;
        case 1:
            place_to_insert = &(par->child[0]->child[2]);
            possible_par = par->child[0];
            break;
        case 2:
            place_to_insert = &(par->child[1]->child[2]);
            possible_par = par->child[1];
    }

    *place_to_insert = node_to_insert;
    if (node_to_insert) 
        (*place_to_insert)->par = possible_par;
}

void clear_par_and_leaf (size_t victim_num, BNodePtr par, BNodePtr leaf) {
    switch (victim_num) {
        case 0:
        case 1:
            shift_infolists_and_change_sz (par, par->info[0]->key);
            break;
        case 2:
            shift_infolists_and_change_sz (par, par->info[1]->key);
    }
    free_nullify (leaf);
}

BNodePtr get_nonnull_child (BNodePtr leaf) {
    if (leaf->child[0]) 
        return leaf->child[0];
    if (leaf->child[1]) 
        return leaf->child[1];
    return NULL;
}

void rotate_right (size_t src, size_t dest, BNodePtr par) {
    /*  right shift leaf children  */
    if (par->child[dest]->child[1] == NULL) {
        par->child[dest]->child[1] = par->child[dest]->child[0];
        par->child[dest]->child[0] = NULL;
    }

    size_t src_sz = par->child[src]->csize;
    /*  move infos  */
    par->child[dest]->info[0] = par->info[src];
    par->child[dest]->csize = 1;

    par->info[src] = par->child[src]->info[src_sz - 1];
    shift_infolists_and_change_sz (par->child[src], par->child[src]->info[src_sz - 1]->key);

    /*  right shift source node children  */
    par->child[dest]->child[0] = par->child[src]->child[src_sz];
    par->child[src]->child[src_sz] = NULL;

    /*  set parents  */
    if (par->child[dest]->child[0]) 
        par->child[dest]->child[0]->par = par->child[dest];
}

void rotate_left (size_t src, size_t dest, BNodePtr par) {
    /*  left shift leaf children  */
    if (par->child[dest]->child[0] == NULL) {
        par->child[dest]->child[0] = par->child[dest]->child[1];
        par->child[dest]->child[1] = NULL;
    }

    size_t src_sz = par->child[src]->csize;
    /*  move infos  */
    par->child[dest]->info[0] = par->info[src - 1];
    par->child[dest]->csize = 1;

    par->info[src - 1] = par->child[src]->info[0];
    shift_infolists_and_change_sz (par->child[src], par->child[src]->info[0]->key);

    /*  left shift source node children  */
    par->child[dest]->child[1] = par->child[src]->child[0];
    for (size_t i = 0; i < src_sz; i++)
        par->child[src]->child[i] = par->child[src]->child[i+1];
    par->child[src]->child[src_sz] = NULL;

    /*  set parents  */
    if (par->child[dest]->child[1]) 
        par->child[dest]->child[1]->par = par->child[dest];
}

size_t set_leaf_num (BNodePtr par, BNodePtr leaf) {

    for (size_t i = 0; i < CHILD_NUM - 1; i++) {
        if (leaf == par->child[i]) 
            return i;
    }
    return CHILD_NUM - 1;
}

/*  Traverse  */
void traverse_bt (BNodePtr root) {
    if (!root) 
        return;

    for (size_t i = 0; i < root->csize; i++) {
        traverse_bt (root->child[i]);
        printf ("%s ", root->info[i]->key);
    }
    traverse_bt (root->child[root->csize]);
}

 
/*  DESTRUCTORS  */
void free_bt (BNodePtr *root) {
    if (!(*root))
        return;

    free_bt (&((*root)->child[0]));
    free_bt (&((*root)->child[1]));
    free_bt (&((*root)->child[2]));

    free_vertex (root);
}



