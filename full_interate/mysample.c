#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"


struct person {
	int id;
	char name[32];
	struct list_head list;
};

//struct list_head head;
//INIT_LIST_HEAD(&head);
LIST_HEAD(head);

struct person * new_person(int id, char *name)
{
	struct person *p = (struct person *)malloc(sizeof(struct person));

	if (!p) {
		fprintf(stderr, "NO MEM\n");
		return NULL;
	}
	p->id = id;
	memcpy(p->name, name, strlen(name));

	return p;
}

int main()
{
	struct person * p, *pp;
	struct list_head *plist;
	
	p = new_person(1, "jerry");
	list_add_tail(&p->list, &head);

	p = new_person(2, "tom");
	list_add_tail(&p->list, &head);

	p = new_person(3, "bob");
	list_add_tail(&p->list, &head);

	p = new_person(4, "jane");
	list_add_tail(&p->list, &head);


	/* struct list_node 遍历 */
	fprintf(stdout, "\tlist_for_each:\n");
	list_for_each(plist, &head) {
		pp = list_entry(plist, struct person, list);
		fprintf(stdout, "ID: %d,\t Name: %s\n", pp->id, pp->name);
	}
	
	/* struct list_node 反向遍历 */
	fprintf(stdout, "\n\tlist_for_each:\n");
	list_for_each_prev(plist, &head) {
		pp = list_entry(plist, struct person, list);
		fprintf(stdout, "ID: %d,\t Name: %s\n", pp->id, pp->name);
	}
	
	/* 结构体遍历 */
	fprintf(stdout, "\n\tlist_for_each_entry:\n");
	list_for_each_entry(pp,  &head, list) {
		fprintf(stdout, "ID: %d,\t Name: %s\n", pp->id, pp->name);
	}

	/* 结构体反向遍历 */
	fprintf(stdout, "\n\tlist_for_each_entry_reverse:\n");
	list_for_each_entry_reverse(pp,  &head, list) {
		fprintf(stdout, "ID: %d,\t Name: %s\n", pp->id, pp->name);
	}
}
