#include<stdint.h>
#include<string.h>
#include<iostream>
#include<stdio.h>
#include <Windows.h>
#include <fstream>

using namespace std;
//typedef __int64		uint64_t;
typedef uint64_t    ngx_atomic_uint_t;
//typedef unsigned int *	uintptr_t;

typedef volatile ngx_atomic_uint_t  ngx_atomic_t;
typedef uintptr_t ngx_uint_t;
//typedef int *	  intptr_t;
typedef int		  ngx_fd_t;
typedef intptr_t  ngx_int_t;
static ngx_uint_t  ngx_slab_max_size = 0;
static ngx_uint_t  ngx_slab_exact_size;
static ngx_uint_t  ngx_slab_exact_shift;


#define ngx_align_ptr(p, a)                                                   \
	(u_char *)(((uintptr_t)(p)+((uintptr_t)a - 1)) & ~((uintptr_t)a - 1))

#define ngx_memset(buf, c, n)     (void) memset(buf, c, n)
#define ngx_memzero(buf, n)       (void) memset(buf, 0, n)

#define ngx_slab_junk(p, size)                                                \
if (ngx_debug_malloc)          ngx_memset(p, 0xA5, size)

#define ngx_strcmp(s1, s2)  strcmp((const char *) s1, (const char *) s2)


typedef struct {
	ngx_atomic_t   lock;
#if (NGX_HAVE_POSIX_SEM)
	ngx_atomic_t   wait;
#endif
} ngx_shmtx_sh_t;

typedef struct ngx_slab_page_s  ngx_slab_page_t;

struct ngx_slab_page_s {
	uintptr_t         slab;
	ngx_slab_page_t  *next;
	uintptr_t         prev;
};


typedef struct {
#if (NGX_HAVE_ATOMIC_OPS)
	ngx_atomic_t  *lock;
#if (NGX_HAVE_POSIX_SEM)
	ngx_atomic_t  *wait;
	ngx_uint_t     semaphore;
	sem_t          sem;
#endif
#else
	ngx_fd_t       fd;
	u_char        *name;
#endif
	ngx_uint_t     spin;
} ngx_shmtx_t;

typedef struct {
	ngx_shmtx_sh_t    lock;

	size_t            min_size;
	size_t            min_shift;

	ngx_slab_page_t  *pages;
	ngx_slab_page_t   free;

	u_char           *start;
	u_char           *end;

	ngx_shmtx_t       mutex;

	u_char           *log_ctx;
	u_char            zero;

	unsigned          log_nomem : 1;

	void             *data;
	void             *addr;
} ngx_slab_pool_t;

typedef struct ngx_shm_zone_s  ngx_shm_zone_t;

typedef ngx_int_t(*ngx_shm_zone_init_pt) (ngx_shm_zone_t *zone, void *data);


typedef struct {
	size_t      len;
	u_char     *data;
} ngx_str_t;

typedef struct {
	u_char      *addr;
	size_t       size;
	ngx_str_t    name;

	ngx_uint_t   exists;   /* unsigned  exists:1;  */
} ngx_shm_t;

struct ngx_shm_zone_s {
	void                     *data;
	ngx_shm_t                 shm;
	ngx_shm_zone_init_pt      init;
	void                     *tag;
};



void ngx_slab_init(ngx_slab_pool_t *pool);