#include<iostream>
#include<string>
using namespace std;


typedef struct mem{
	void * p;
	void * start;
	void * end;
	void * free;

}mem;

mem * mem1 = new mem;

void init(mem * mem1)
{
	
	mem1->p = new char[10];
	mem1->start = mem1->p;
	mem1->end = (char *)mem1->p + (10 * sizeof(char));
	mem1->free = mem1->p;

}

char* alloc(char* p, int num)
{
	p = (char*)(mem1->start) + num;
	mem1->start = (char*)mem1->start + num;
	return p;
}
typedef struct test{
	char * name;
}test;

int main_memorySimple()
{
	
	init(mem1);
	test* test1 = new test;
	
	test1->name = alloc(test1->name, 10);
	
	strcpy(test1->name, "test");

	cout << test1->name << endl;

	
	system("pause");
	return 0;
}