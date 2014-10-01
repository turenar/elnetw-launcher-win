/*
 * node.c
 *
 *  Created on: 2013/10/12
 *      Author: Turenar (snswinhaiku dot lo at gmail dot com)
 */

#include "config.inc.h"
#include <stdlib.h>
#include "node.h"

typedef struct _link_node llnode;

llist *node_init() {
	llist *list = calloc(1, sizeof(llist));
	return list;
}

void node_add_list(llist *list, const void *obj) {
	llnode *new_node = (llnode*) calloc(1, sizeof(llnode));
	if (list->first == NULL) {
		list->first = list->last = new_node;
	} else {
		llnode *last = list->last;
		list->last = last->next = new_node;
	}
	new_node->obj = obj;
	printf("%d", list->size);
	list->size++;
}

void** node_to_array(llist *list) {
	int size = list->size;
	void** arr = (void**) malloc(sizeof(void*) * (size + 1));

	int i = 0;
	const void** cur = (void*) arr;
	llnode* nod = list->first;
	for (; i < size; i++) {
		*cur++ = nod->obj;
		nod = nod->next;
	}
	*cur = NULL;
	return arr;
}

llist *node_add_all(llist *dst, const llist *src) {
	llnode *pointer = src->first;
	while (pointer != NULL) {
		node_add_list(dst, pointer->obj);
		pointer = pointer->next;
	}
	return dst;
}

LPTSTR node_combine_str(llist *list, LPCTSTR glue) {
	int glue_len = _tcslen(glue);
	int len = 0;
	llnode *pointer = list->first;
	while (pointer != NULL) {
		len += _tcslen(pointer->obj) + glue_len;
		pointer = pointer->next;
	}
	len -= glue_len;

	LPTSTR string = malloc((len + 1) * sizeof(TCHAR));

	pointer = list->first;
	TCHAR* str_lp = string;
	while (pointer != NULL) {
		const TCHAR* str = (const TCHAR*) pointer->obj;
		while ((*str) != _T('\0')) {
			*(str_lp++) = *(str++);
		}
		pointer = pointer->next;

		if (pointer != NULL) {
			str = glue;
			while ((*str) != _T('\0')) {
				*(str_lp++) = *(str++);
			}
		}
	}
	*str_lp = _T('\0');
	return string;
}

