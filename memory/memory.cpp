#include"memory.h"

ngx_uint_t  ngx_pagesize;
ngx_uint_t  ngx_pagesize_shift;
ngx_uint_t  ngx_cacheline_size;
ngx_uint_t  ngx_debug_malloc;




void ngx_slab_init(ngx_slab_pool_t *pool)
{
	u_char           *p;
	size_t            size;
	ngx_int_t         m;
	ngx_uint_t        i, n, pages;
	ngx_slab_page_t  *slots;

	/* STUB */
	if (ngx_slab_max_size == 0) {
		ngx_slab_max_size = ngx_pagesize / 2;
		ngx_slab_exact_size = ngx_pagesize / (8 * sizeof(uintptr_t));
		for (n = ngx_slab_exact_size; n >>= 1; ngx_slab_exact_shift++) {
			/* void */
		}
	}
	/**/

	pool->min_size = 1 << pool->min_shift;

	p = (u_char *)pool + sizeof(ngx_slab_pool_t);
	size = pool->end - p;

	ngx_slab_junk(p, size);

	slots = (ngx_slab_page_t *)p;
	n = ngx_pagesize_shift - pool->min_shift;

	for (i = 0; i < n; i++) {
		slots[i].slab = 0;
		slots[i].next = &slots[i];
		slots[i].prev = 0;
	}

	p += n * sizeof(ngx_slab_page_t);

	pages = (ngx_uint_t)(size / (ngx_pagesize + sizeof(ngx_slab_page_t)));

	ngx_memzero(p, pages * sizeof(ngx_slab_page_t));

	pool->pages = (ngx_slab_page_t *)p;

	pool->free.prev = 0;
	pool->free.next = (ngx_slab_page_t *)p;

	pool->pages->slab = pages;
	pool->pages->next = &pool->free;
	pool->pages->prev = (uintptr_t)&pool->free;

	pool->start = (u_char *)
		ngx_align_ptr((uintptr_t)p + pages * sizeof(ngx_slab_page_t),
		ngx_pagesize);

	m = pages - (pool->end - pool->start) / ngx_pagesize;
	if (m > 0) {
		pages -= m;
		pool->pages->slab = pages;
	}

	pool->log_nomem = 1;
	pool->log_ctx = &pool->zero;
	pool->zero = '\0';
}
