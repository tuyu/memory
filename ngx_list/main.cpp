#include <fstream>
typedef uintptr_t       ngx_uint_t;
typedef struct ngx_list_part_s  ngx_list_part_t;

struct ngx_list_part_s {
	void             *elts; //������־���һ�����飬�������������ڴ�洢����
	ngx_uint_t        nelts; //�洢�˶��ٸ�����
	ngx_list_part_t  *next; //��һ������
};

//�봫ͳ��list�����Ĳ�֮ͬ�����ڣ���ͳ��list����ֻ�ܴ��һ��Ԫ�أ�ngx_list_t�Ľڵ�ʵ������һ���̶���С������
typedef struct {
	ngx_list_part_t  *last; //ָ�����
	ngx_list_part_t   part;
	size_t            size;
	ngx_uint_t        nalloc;
	//ngx_pool_t       *pool;
	char				*pool;
} ngx_list_t;