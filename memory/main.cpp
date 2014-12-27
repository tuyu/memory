#include<iostream>
#include"memory.h"
using namespace std;

int main_memory()
{
	ngx_slab_pool_t  pool ;
	pool.pages = new ngx_slab_page_t;
	ngx_slab_init(&pool);
	return 0;
}