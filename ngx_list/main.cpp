#include <fstream>
#include <string>
#include <iostream>

using namespace std;

typedef uintptr_t       ngx_uint_t;
typedef struct ngx_list_part_s  ngx_list_part_t;

typedef struct ngx_list_part_s {
	void             *elts; //这个部分就是一个数组，可以用连续的内存存储数据
	ngx_uint_t        nelts; //存储了多少个数据,该字段不能大于ngx_list_t的nalloc字段
	ngx_list_part_t  *next; //下一个部分
}ngx_list_part_s;

//与传统的list容器的不同之处在于：传统的list容器只能存放一个元素，ngx_list_t的节点实际上是一个固定大小的数组
typedef struct {
	ngx_list_part_t  *last; //指向该链表的最后的一个节点
	ngx_list_part_t   part; //该链表的首个存放具体元素的结点
	size_t            size; //链表中存放的具体元素所需内存大小
	ngx_uint_t        nalloc; //每个节点所含的固定大小的数组的容量
	//ngx_pool_t       *pool; //该list使用的分配内存的pool
	char				*pool; 
} ngx_list_t;

typedef struct elem{
	int usrno;
}elem;

ngx_list_t* ngx_list_create(char* place, int n, int size)
{
	ngx_list_t* list;
	list = new ngx_list_t;
	if (list == NULL)
		return NULL;

	list->part.elts = new char[n*size];//分配该list中的存储结构的成员的存储内存
	if (list->part.elts == NULL)
		return NULL;

	list->part.nelts = 0;
	list->part.next = NULL;
	list->size = size; //每个成员的大小
	list->nalloc = n; //有多少个这样的成员
	list->pool = place;
	list->last = &list->part;

	return list;

}

void * ngx_list_push(ngx_list_t *l)
{
	void * elt;
	ngx_list_part_t * last;
	last = l->last;

	if (last->nelts == l->nalloc) //此处判断最后一个part是否已满，如果已满那么重新分配一个list part
	{
		last = new ngx_list_part_t;
		if (last == NULL)
			return NULL;

		last->elts = new char[l->nalloc * l->size];
		if (last->elts == NULL)
			return NULL;

		last->nelts == 0; //此时还是一个元素都没有
		last->next = NULL; //下一个节点为空
		
		l->last->next = last;
		l->last = last; 
	}

	elt = (char*)last->elts + l->size*last->nelts;//找到有第几个位置是空闲的，那么将在这个位置存储对应的数据，并且记录下这个元素
	last->nelts++;

	return elt; //然后返回这个位置的指针，接着外部程序就可以进行赋值操作

}

void print(ngx_list_t *l)
{
	
	ngx_list_part_s *temp;
	temp = &(l->part);
	do{
		int i = l->part.nelts;
		for (int j = 0; j < i; j++)
			cout << ((elem*)((char*)temp->elts + j*sizeof(elem)))->usrno << endl; //如果没有(char*),那么每次遍历都会走一个10，需要查看原因

		temp = l->part.next;
	} while (temp != NULL);

	
}

int main()
{
	char * test = new char[10];
	strcpy(test, "hello");

	ngx_list_t * t;
	t = ngx_list_create(test, 10, sizeof(elem));

	elem* temp = NULL;
	for (int i = 0; i < 10; i++)
	{
		temp = (elem*)ngx_list_push(t);
		temp->usrno = i;
	}

	print(t);


	system("pause");
	return 0;
}