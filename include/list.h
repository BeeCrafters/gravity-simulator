#include <stdint.h>
#include <stddef.h>

#ifndef __LIST_H__
#define __LIST_H__

// Intrusive list definitions

typedef struct list_node_s {
	struct list_node_s* prev;
	struct list_node_s* next;
} list_node;

#define container_of(ptr, type, member) \
	(( type * ) ( ((uint8_t*) (ptr) ) - offsetof(type, member) ) )

#define list_for_each(pos, head, member) \
	for ( \
		pos = container_of((head)->next, typeof(*pos), member); \
		&(pos->member) != (head); \
		pos = container_of(pos->member.next, typeof(*pos), member) \
	)


#define list_for_each_safe(pos, n, head, member) \
	for ( \
		pos = container_of((head)->next, typeof(*pos), member), \
		n = container_of(pos->member.next, typeof(*pos), member); \
		&(pos->member) != (head); \
		pos = n, n = container_of(n->member.next, typeof(*n), member) \
	)

#define list_is_empty(head) ((head)->next == (head))

#define list_next(head, type, member) \
	container_of((head)->next, type, member)

#define list_prev(head, type, member) \
	container_of((head)->prev, type, member)

#endif
