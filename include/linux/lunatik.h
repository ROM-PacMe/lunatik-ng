#ifndef _LUNATIK_H
#define _LUNATIK_H

/*
 * lunatik.h
 * See Copyright Notice at the end of this file.
 */

#include <linux/mutex.h>
#include <linux/lunatik/lauxlib.h>

struct lunatik_result {
	/*
	 * Supported return types:
	 *
	 * - LUA_TNIL (no further details)
	 * - LUA_TNUMBER (value in r_number)
	 * - LUA_TBOOLEAN (value in r_boolean)
	 * - LUA_TSTRING (copy in r_string, length in r_string_size)
	 * - LUA_TUSERDATA (copy data in r_userdata, length in r_userdata_size,
	 *   name of metatable in r_userdata_type)
	 * - LUA_TLIGHTUSERDATA (pointer value in r_lightuserdata)
	 */
	int r_type;
	union {
		s64 r_number;
		int r_boolean;
		struct {
			char *r_string;
			size_t r_string_size;
		};
		struct {
			void *r_userdata;
			size_t r_userdata_size;
			char *r_userdata_type;
		};
		void *r_lightuserdata;
	};
};

struct lunatik_workqueue {
	struct workqueue_struct *wq;
};

struct lunatik_context {
	lua_State *L;
	struct mutex mutex;
	struct lunatik_workqueue lwq;
	struct list_head active_bindings;
};

typedef int (*lunatik_binding_func)(struct lunatik_context *lc);

struct lunatik_binding {
	struct module *owner;
	lunatik_binding_func regfunc;
	struct list_head link;
};

typedef void (*lunatik_loadcode_callback)(void *arg, int ret,
					struct lunatik_result *r);
typedef void (*lunatik_loadcode_callback_nores)(void *arg, int ret);

extern struct lunatik_context *lunatik_context_create(char *name);
extern void lunatik_context_destroy(struct lunatik_context *lc);
#define lunatik_context_lock(context_ptr) mutex_lock(&(context_ptr)->mutex);
#define lunatik_context_unlock(context_ptr) mutex_unlock(&(context_ptr)->mutex);
struct lunatik_context *lunatik_default_context_get(void);

/* DEPRECATED - use direct, sync, or async variant instead */
extern int lunatik_loadcode(struct lunatik_context *lc, char *code,
			size_t sz_code, struct lunatik_result **presult);
extern int lunatik_loadcode_direct(struct lunatik_context *lc, char *code,
				size_t sz_code,
				struct lunatik_result **presult);
extern int lunatik_loadcode_sync(struct lunatik_context *lc, char *code,
				size_t sz_code,
				struct lunatik_result **presult);
extern int lunatik_loadcode_async(struct lunatik_context *lc, char *code,
				size_t sz_code,
				lunatik_loadcode_callback callb,
				void *callb_arg);
extern int lunatik_loadcode_async_nores(struct lunatik_context *lc, char *code,
					size_t sz_code,
					lunatik_loadcode_callback_nores callb,
					void *callb_arg);
extern void lunatik_result_free(const struct lunatik_result *result);
extern int lunatik_openlib(struct lunatik_context *lc,
			lua_CFunction luaopen_func);
extern struct lunatik_binding *lunatik_bindings_register(
	struct module *owner, lunatik_binding_func regfunc);
extern void lunatik_bindings_unregister(struct lunatik_binding *b);
extern int lunatik_bindings_load(struct lunatik_context *lc);

/******************************************************************************
 *  Copyright (C) 2009 Lourival Vieira Neto.  All rights reserved.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 *  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ******************************************************************************/

#endif /* _LUNATIK_H */
