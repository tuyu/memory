#include <fstream>
typedef uintptr_t       ngx_uint_t;
typedef struct ngx_list_part_s  ngx_list_part_t;

struct ngx_list_part_s {
	void             *elts; //这个部分就是一个数组，可以用连续的内存存储数据
	ngx_uint_t        nelts; //存储了多少个数据
	ngx_list_part_t  *next; //下一个部分
};

//与传统的list容器的不同之处在于：传统的list容器只能存放一个元素，ngx_list_t的节点实际上是一个固定大小的数组
typedef struct {
	ngx_list_part_t  *last; //指向最后
	ngx_list_part_t   part;
	size_t            size;
	ngx_uint_t        nalloc;
	//ngx_pool_t       *pool;
	char				*pool;
} ngx_list_t;