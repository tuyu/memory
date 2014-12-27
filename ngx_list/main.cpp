#include <fstream>
#include <string>
#include <iostream>

using namespace std;

typedef uintptr_t       ngx_uint_t;
typedef struct ngx_list_part_s  ngx_list_part_t;

typedef struct ngx_list_part_s {
	void             *elts; //������־���һ�����飬�������������ڴ�洢����
	ngx_uint_t        nelts; //�洢�˶��ٸ�����,���ֶβ��ܴ���ngx_list_t��nalloc�ֶ�
	ngx_list_part_t  *next; //��һ������
}ngx_list_part_s;

//�봫ͳ��list�����Ĳ�֮ͬ�����ڣ���ͳ��list����ֻ�ܴ��һ��Ԫ�أ�ngx_list_t�Ľڵ�ʵ������һ���̶���С������
typedef struct {
	ngx_list_part_t  *last; //ָ������������һ���ڵ�
	ngx_list_part_t   part; //��������׸���ž���Ԫ�صĽ��
	size_t            size; //�����д�ŵľ���Ԫ�������ڴ��С
	ngx_uint_t        nalloc; //ÿ���ڵ������Ĺ̶���С�����������
	//ngx_pool_t       *pool; //��listʹ�õķ����ڴ��pool
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

	list->part.elts = new char[n*size];//�����list�еĴ洢�ṹ�ĳ�Ա�Ĵ洢�ڴ�
	if (list->part.elts == NULL)
		return NULL;

	list->part.nelts = 0;
	list->part.next = NULL;
	list->size = size; //ÿ����Ա�Ĵ�С
	list->nalloc = n; //�ж��ٸ������ĳ�Ա
	list->pool = place;
	list->last = &list->part;

	return list;

}

void * ngx_list_push(ngx_list_t *l)
{
	void * elt;
	ngx_list_part_t * last;
	last = l->last;

	if (last->nelts == l->nalloc) //�˴��ж����һ��part�Ƿ����������������ô���·���һ��list part
	{
		last = new ngx_list_part_t;
		if (last == NULL)
			return NULL;

		last->elts = new char[l->nalloc * l->size];
		if (last->elts == NULL)
			return NULL;

		last->nelts == 0; //��ʱ����һ��Ԫ�ض�û��
		last->next = NULL; //��һ���ڵ�Ϊ��
		
		l->last->next = last;
		l->last = last; 
	}

	elt = (char*)last->elts + l->size*last->nelts;//�ҵ��еڼ���λ���ǿ��еģ���ô�������λ�ô洢��Ӧ�����ݣ����Ҽ�¼�����Ԫ��
	last->nelts++;

	return elt; //Ȼ�󷵻����λ�õ�ָ�룬�����ⲿ����Ϳ��Խ��и�ֵ����

}

void print(ngx_list_t *l)
{
	
	ngx_list_part_s *temp;
	temp = &(l->part);
	do{
		int i = l->part.nelts;
		for (int j = 0; j < i; j++)
			cout << ((elem*)((char*)temp->elts + j*sizeof(elem)))->usrno << endl; //���û��(char*),��ôÿ�α���������һ��10����Ҫ�鿴ԭ��

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