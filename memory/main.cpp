#include<iostream>
#include"memory.h"
using namespace std;

int main()
{
	ngx_slab_pool_t  *pool = NULL ;

	pool = (ngx_slab_pool_t* )new char[10000];
	pool->addr = (ngx_slab_pool_t*)pool;

	pool->end = (u_char*)((char*)pool->addr + sizeof(char)* 10000);



	//pool.pages = new ngx_slab_page_t;

	ngx_slab_init(pool);
	return 0;
}