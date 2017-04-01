## list节点定义
Linux实现的list结构不同于我们常见的实现方式。通常，链表包含它要连接元素，如 ：
```c
struct my_list {
    void *myitem;
    struct my_list *next;
    struct my_list *prev;
};
```
Linux的实现是链表包含在元素中，例如，你要创建struct my_cool_list，可以按照如下方式：
```c
struct my_cool_list{
    struct list_head list;
    int my_cool_data;
    void *my_cool_void;
};
```
需要注意：
- list在你要连在一起的数据项中
- 可以放置struct list_head到结构体的任何位置
- 可以为struct list_head赋任何变量名
- 可以有多个链表

例如，下面的声明是正确的：
```c
struct todo_tasks {
    char *task_name;
    unsigned int name_len;
    short int status;

    int sub_tasks;

    int subtasks_completed;
    struct list_head completed_subtasks; /*链表结构*/

    int sub_tasks_waiting;
    struct list_head waiting_subtasks;

    struct list_head todo_list;
};
```
内核的list结构这样声明：
```c
	struct list_head {
		struct list_head *next;
		struct list_head *prev;
	};
```
下面，我们看如何使用内核的list实现，然后解释具体实现。

## list 使用方式
下面示例，展示了如何对链表执行创建，插入，删除，和遍历操作，源文件：
```c
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

struct kool_list{
	int to;
	struct list_head list;
	int from;
};

int main(int argc, char **argv)
{
	struct kool_list *tmp;
	struct list_head *pos, *q;
	unsigned int i;

	sturct kool_list mylist;
	INIT_LIST_HEAD(&mylist.list);

	/* 向mylist添加元素 */
	for( i=5; i!=0; --i ){
		tmp = (struct kool_list *)malloc(sizeof(struct kool_list));

		printf("enter to and from:");
		scanf("%d %d", &temp->to, &temp->from);

		/* 添加tmp到mylist链表中 */
		list_add(&(temp->list), &(mylist.list));
		/* 你可以使用list_add_tail()，将新元素添加到链表尾部 */
	}
	printf("\n");

	/* 现在我们有个循环链表，接下来就是遍历链表，并将结果打印出来 */
	printf("使用list_for_each()遍历这个数组");
	list_for_each(pos, &mylist.list) {

		tmp = list_entry(pos, struct kool_list, list);
		printf("to=%d from=%d\n", tmp->to, tmp->from);
	}
	printf("\n");

	/* 也可以使用list_for_each_entry()来遍历 */
	printf("使用list_for_each_entry()遍历链表\n");
	list_for_each_entry(tmp, &mylist.list, list)
		printf("to=%d from=%d\n", tmp->to, tmp->from);

	/* 现在释放kool_list中元素，因为我们要使用list_del()从链表中删除元素，我们需要使用
		list_for_each()更安全的版本list_for_each_safe(),如果涉及删除或移动元素时，要使用这个宏
	 */
	 list_for_each_safe(pos, q, &mylist.list) {
	 	tmp = list_entry(pos, struct kool_list, list);
	 	printf("释放to=%d from=%d\n", tmp->to, tmp->from);
	 	list_del(pos);
	 	free(tmp);
	 }

	 return 0;
}
```
在给定list指针(struct list_head list)时，我们如何获取包含这个list指针元素的地址。在上面通过list_entry()获取:
```c
/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)
```
下面是container_of的定义:
```c
/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})
```
如上，我们使用
```c
	tmp = list_entry(pos, struct kool_list, list);
```
获取结构的指针，展开后，
```c
	tmp = ((struct kool_list *)((char *)pos - (unsigned long)(&((struct kool_list *)0)->list)))
```
给一个包含在结构体中的struct list_head的指针，宏list_entry()简单地计算出指向结构体的指针，我们就需要知道struct list_head变量在结构体中的位置,(list_head的偏移量),然后只需从传入宏的地址中，减去list_head的偏移量，就可得到结构体的地址。

## 结构体成员偏移量
下面的问题是，我们如何计算结构体成员在结构体中的偏移量，假设有结构体struct foo_bar，你想计算它的成员boo在结构体中的偏移量，可以这样计算:
```c
   (unsigned long)(&((struct foo_bar *)0)->boo)
```
将地址0，类型转换为任意结构类型--在这里是struct foo_bar，然后取成员的地址，这样就得到了成员在结构体中的偏移量。

我们已经知道这个成员的绝对地址(如上面的pos)，减去这个成员的偏移量，即可得到结构体的地址。

为更好理解成员函数偏移量的计算，看下面的示例：
```c
#include <stdio.h>

#include <stddef.h>
//#define offsetof(type, member) ((unsigned long)&((type *)0)->member)

struct foobar {
	unsigned int foo;
	char bar;
	char boo;
};

int main()
{
	struct foobar tmp;

	printf("address of &tmp is = %p \n\n", &tmp);
	printf("address of tmp->foo = %p \t offset of tmp->foo = %lu\n", &tmp.foo, (unsigned long)&((struct foobar *)0)->foo);
	printf("address of tmp->bar = %p \t offset of tmp->bar = %lu\n", &tmp.bar, (unsigned long)&((struct foobar *)0)->bar);
	printf("address of tmp->boo = %p \t offset of tmp->boo = %lu\n", &tmp.boo, (unsigned long)&((struct foobar *)0)->boo);

	printf("\ncomputed address of &tmp using:\n");
	printf("address and offset of tmp->foo = %p\n",
			(struct foobar *)( ((char *) &tmp.foo) - offsetof(struct foobar, foo)) );
	printf("address and offset of tmp->bar = %p\n",
			(struct foobar *)( ((char *) &tmp.bar) - offsetof(struct foobar, bar)) );
	printf("address and offset of tmp->boo = %p\n",
			(struct foobar *)( ((char *) &tmp.boo) - offsetof(struct foobar, boo)) );
}
```

## 结构体的存储

结构体在内存中是如存储的呢？编译器按照成员列表的顺序一个接一个地给每个成员分配内存。只有当存储成员时需要满足正确的边界对齐要求时，成员之间才可能出现用于填充的额外内存空间。为了说明这一点，考虑下面这个结构
```c
struct ALIGN {
    char a;
    int b;
    char c;
};
```
如果某个机器的整型值长度为4个字节，并且它的起始存储位置必须能够被4整除，那么这个结构在内存中的存储如下所示： TODO 插入图片 系统禁止编译器在一个结构的其实位置逃过几个字节来满足边界对齐要求，因此所有结构的起始存储位置必须是结构中边界要求最严格的数据类型所要求的位置。因此，成员a(最左边的那个方框)必须存储于一个能够被4整除的地址，结构的下一个成员是一个整型值，所有它必须跳过3个字节，到达合适的边界才能存储。在整型之后是最后一个字符。

你可以在声明中对结构的成员列表重新排列，让哪些对边界要求最严格的成员首先出现，对边界要求最弱的成员最后出现。这种做法可以最大限度地减少因边界对齐而带来的内存损失。

sizeof操作符能够得到一个结构体的整体长度，包括因边界对齐而跳过的那些字节。如果你必须确定结构某个成员的实际位置，应该考虑边界对齐因素，可以使用offsetof宏(定义在stddef.h)。
```c
	offsetof(type, memeber)
```
type就是结构体的类型，member就是你需要的那个成员名。表达式的结果是一个size_t值，表示这个指定成员开始存储的位置距离结构开始存储位置偏移几个字节。例如，对前面的那个声明而言，
```c
	offsetof(struct ALIGN, b)
```
的返回值是4

## 结构成员存储布局

C语言编译器被限制成按照严格的升序为成员分配递增的内存地址，第一个成员从结构开始地址开始。

连续成员之间和最后一个成员之后可能出现空穴或填充区，使成员在内存中适当对齐。这种填充区中出现的位模式是不确定的，不同结构可能不同，同一结构在不同时间也可能不同。sizeof运算符返回的值包括填充区所占的空间。有些实现提供杂注或开关控制结构成员的存储。
