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
	struct person * pp;
	struct list_head *plist;
	
	pp = new_person(4, "jane");

	plist = &pp->list;

	/* 获取结构体指针 */
	pp = list_entry(plist, struct person, list);
	fprintf(stdout, "ID: %d, \t Name: %s\n", pp->id, pp->name);

}
