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
	struct person * p, *pp, *tmp;
	struct list_head *plist;
	
	p = new_person(1, "jerry");
	list_add_tail(&p->list, &head);

	p = new_person(2, "tom");
	list_add_tail(&p->list, &head);

	p = new_person(3, "bob");
	list_add_tail(&p->list, &head);

	p = new_person(4, "jane");
	list_add_tail(&p->list, &head);


	/* 将第二个元素作为遍历起始元素 */
	p = list_first_entry(&head, struct person, list);
	p = list_next_entry(p, list);
	pp = p; /* 使用pp进行遍历 */

	/* 结构体遍历 */
	fprintf(stdout, "\n\tlist_for_each_continue:\n");
	list_for_each_entry_continue(pp,  &head, list) {
		fprintf(stdout, "ID: %d,\t Name: %s\n", pp->id, pp->name);
	}

	/* 重新为p赋值 */
	pp = p;
	/* 结构体反向遍历 */
	fprintf(stdout, "\n\tlist_for_each_entry_continue_reverse:\n");
	list_for_each_entry_continue_reverse(pp,  &head, list) {
		fprintf(stdout, "ID: %d,\t Name: %s\n", pp->id, pp->name);
	}

	/* 重新为p赋值 */
	pp = p;
	/* 结构体遍历 */
	fprintf(stdout, "\n\tlist_for_each_entry_from\n");
	list_for_each_entry_from(pp,  &head, list) {
		fprintf(stdout, "ID: %d,\t Name: %s\n", pp->id, pp->name);
	}

    /* clean environment */
    list_for_each_entry_safe(pp, tmp, &head, list) {
        list_del(&pp->list);   
        free(pp);
    }
}
