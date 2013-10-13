/*
 * node.h
 *
 *  Created on: 2013/09/17
 *      Author: Turenar (snswinhaiku dot lo at gmail dot com)
 */

#ifndef NODE_H_
#define NODE_H_

struct _link_node {
	const void *obj;
	struct _link_node *next;
};
typedef struct _linked_list {
	int size;
	struct _link_node *first;
	struct _link_node *last;
} llist;

llist *node_init() __attribute__((malloc));
void node_add_list(llist *list, const void *obj);
void** node_to_array(llist *list) __attribute__((malloc));
llist *node_add_all(llist *dst, const llist *src);

#endif /* NODE_H_ */
