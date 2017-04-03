#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
		assert(0);
		return NULL;
	}
	p->id = id;
	memcpy(p->name, name, strlen(name));

	return p;
}

int main()
{
	struct person *pp, *first, *last, *tmp;

	pp = new_person(1, "jerry");
	list_add_tail(&pp->list, &head);

	pp = new_person(2, "tom");
	list_add_tail(&pp->list, &head);


	pp = new_person(3, "bob");
	list_add_tail(&pp->list, &head);

	pp = new_person(4, "jane");
	list_add_tail(&pp->list, &head);


	first = list_first_entry(&head, struct person, list);	// for list
	last = list_last_entry(&head, struct person, list);
	
	fprintf(stdout, "ID: %d,\t Name: %s\n", first->id, first->name);
	fprintf(stdout, "ID: %d,\t Name: %s\n", last->id, last->name);

	/* clean environment */
	list_for_each_entry_safe(pp, tmp, &head, list) {
		list_del(&pp->list);	
		free(pp);
	}
}
