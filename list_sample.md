### 说明
源码中，宏定义的相关名字作用
- pos： 元素对象指针
- member： 结构体struct list_head在嵌套结构体中的成员名
- head： 链表的头
- ptr : 结构体struct list_head的指针

### 由 list_head 成员指针，获取结构体指针
```c
/**
 * list_entry - get the struct for this entry
 */
#define list_entry(ptr, type, member) \
container_of(ptr, type, member)
```
由下面的代码：
```c
struct person *p  = new_person(1, "jane");
struct list_head *plist;
/* 为plist赋一个值 */
plist = &p->list;
/* 通过list_head成员，获取对应结构体指针 */
struct person *pp = list_entry(plist, struct person, list);

```

### 获取两端元素
```c
/**
 * list_first_entry - get the first element from a list
 * Note, that list is expected to be not empty.
 */
#define list_first_entry(ptr, type, member) \
    list_entry((ptr)->next, type, member)

/**
 * list_last_entry - get the last element from a list
 * Note, that list is expected to be not empty.
 */
#define list_last_entry(ptr, type, member) \
    list_entry((ptr)->prev, type, member)

```
如下面代码：
```c
//第一个元素
pp = list_first_entry(&head, struct person, list);
// 最后一个元素
pp = list_last_entry(&head, struct person, list);
```
有意思的是给定一个链表头指针，不能确定它是否真的为链表头指针，告诉函数链表成员pp的list指针&pp->list为链表头指针，可能它也发现不了。

### 获取指定元素的前后元素
```c
/**
 * list_next_entry - get the next element in list
 */
#define list_next_entry(pos, member) \
    list_entry((pos)->member.next, typeof(*(pos)), member)

/**
 * list_prev_entry - get the prev element in list
 */
#define list_prev_entry(pos, member) \
    list_entry((pos)->member.prev, typeof(*(pos)), member)

```
如下面的代码：
```c
struct person *pp, *prev, *next;
/* pp 指向链表第二个元素 */
pp = list_first_entry(&head, struct person, list);
pp = list_next_entry(pp, list);

/* 获取 pp 前一个元素 */
prev = list_prev_entry(pp, list);

/* 获取 pp 后一个元素 */
next = list_next_entry(pp, list);
```

## 遍历链表

### 完全遍历
完全遍历分为两类，一类根据链表头遍历，另一类根据链表结构体对象遍历
#### 链表头遍历
分四种情况：

  |  非删除安全 | 删除安全
 --- |: --- :| ---
 正向遍历 | list_for_each | list_for_each_safe
 反向遍历 | list_for_each_prev | list_for_each_prev_safe

#### 结构体遍历
分四种情况：

  |  非删除安全 | 删除安全
 --- |: --- :| ---
 正向遍历 | list_for_each_entry | list_for_each_entry_safe
 反向遍历 | list_for_each_entry_reverse | list_for_each_entry_reverse_safe

### 非完全遍历

#### 准备起始点
```c
/**
 * list_prepare_entry - prepare a pos entry for use in list_for_each_entry_continue()
 * Prepares a pos entry for use as a start point in list_for_each_entry_continue().
 */
#define list_prepare_entry(pos, head, member) \
    ((pos) ? : list_entry(head, typeof(*pos), member))
```
准备一个结构体指针
```c
    /* 确保pp不为空 */
    struct person * pp = new_person(1, "jane");
    pp = list_prepare_entry(pp, &head, list);
```
删除分以下几种情况：

  |  非删除安全 | 删除安全
 --- |: --- :| ---
 正向遍历 | list_for_each_entry_continue | list_for_each_entry_continue_safe
 反向遍历 | list_for_each_entry_continue_reverse |
