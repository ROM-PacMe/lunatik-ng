#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/lunatik.h>

int lunatik_printk(lua_State *L) {
	int n = lua_gettop(L);
	int i;

	for (i = n; i >= 1; i--) {
		const char *s = lua_tostring(L, -i);
		if (s)
			printk("%s", s);
		else
			printk("(null)");
	}

	if (n != 0)
		printk("\n");

	lua_pop(L, n);

	return 0;
}
EXPORT_SYMBOL(lunatik_printk);

static int __init lunatik_printk_init(void)
{
	lua_State *L = lunatik_get_global_state();

	lua_register(L, "print", &lunatik_printk);

	return 0;
}

MODULE_AUTHOR("Matthias Grawinkel <grawinkel@uni-mainz.de>, Daniel Bausch <bausch@dvs.tu-darmstadt.de>");
MODULE_LICENSE("Dual MIT/GPL");

module_init(lunatik_printk_init);
