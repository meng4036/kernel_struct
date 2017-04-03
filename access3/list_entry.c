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
	struct person *pp, *prev, *next;

	pp = new_person(1, "jerry");
	list_add_tail(&pp->list, &head);

	pp = new_person(2, "tom");
	list_add_tail(&pp->list, &head);


	pp = new_person(3, "bob");
	list_add_tail(&pp->list, &head);

	pp = new_person(4, "jane");
	list_add_tail(&pp->list, &head);

	/* 将pp指向第二个元素 */
	pp = list_first_entry(&head, struct person, list);	
	pp = list_next_entry(pp, list);

	/* 获取pp前一个元素 */
	prev = list_prev_entry(pp, list);
	/* 获取pp后一个元素 */
	next = list_next_entry(pp, list);

	fprintf(stdout, "ID: %d,\t Name: %s\n", prev->id, prev->name);
	fprintf(stdout, "ID: %d,\t Name: %s\n", next->id, next->name);

}
