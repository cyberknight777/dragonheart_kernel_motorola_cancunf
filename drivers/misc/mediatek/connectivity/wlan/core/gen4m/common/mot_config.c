#include "mot_config.h"
#include <linux/string.h>
/* for reading moto bootargs */
#include <linux/of.h>

// BEGIN IKSWR-130356
typedef struct moto_product {
	char hw_device[ARRAY_VALUE_MAX];
	char hw_radio[ARRAY_VALUE_MAX];
	char wifi_cfg_name[ARRAY_VALUE_MAX];
	char txpowerctrl_name[ARRAY_VALUE_MAX];
} moto_product;

static moto_product products_list[] = {
		{"ellis",    "NCA", "wifi",     "txpowerctrl"},
		{"ellis",    "CA",  "wifi_EPA", "txpowerctrl_EPA"},
		{"saipan",   "all", "wifi",     "txpowerctrl"},
		{"tesla",    "NA", "wifi",     "txpowerctrl_na"},
		{"tesla",    "all", "wifi",     "txpowerctrl"},
		{"lyriq",   "JAPAN", "wifi",     "txpowerctrl_JAPAN"},
		{"maui",    "NA",  "wifi_epa", "txpowerctrl_EPA"},
		{"maui",    "NA_CARRIERS",  "wifi", "txpowerctrl"},
		{{0},        {0},   {0},	{0}},
};


/* use moto bootargs to get device name & radio parameters */
static char *bootargs_str = NULL;

static int mmi_get_bootarg_dt(char *key, char **value, char *prop, char *spl_flag)
{
	const char *bootargs_tmp = NULL;
	char *idx = NULL;
	char *kvpair = NULL;
	int err = 1;
	struct device_node *n = of_find_node_by_path("/chosen");
	size_t bootargs_tmp_len = 0;

	if (n == NULL)
		goto err;

	if (of_property_read_string(n, prop, &bootargs_tmp) != 0)
		goto putnode;

	bootargs_tmp_len = strlen(bootargs_tmp);
	if (!bootargs_str) {
		/* The following operations need a non-const
		 * version of bootargs
		 */
		bootargs_str = kzalloc(bootargs_tmp_len + 1, GFP_KERNEL);
		if (!bootargs_str)
			goto putnode;
	}
	strlcpy(bootargs_str, bootargs_tmp, bootargs_tmp_len + 1);

	idx = strnstr(bootargs_str, key, strlen(bootargs_str));
	if (idx) {
		kvpair = strsep(&idx, " ");
		if (kvpair)
			if (strsep(&kvpair, "=")) {
				*value = strsep(&kvpair, spl_flag);
				if (*value)
					err = 0;
			}
	}

putnode:
	of_node_put(n);
err:
	return err;
}

int mmi_get_bootarg(char *key, char **value)
{
#ifdef CONFIG_BOOT_CONFIG
	return mmi_get_bootarg_dt(key, value, "mmi,bootconfig", "\n");
#else
	return mmi_get_bootarg_dt(key, value, "bootargs", " ");
#endif
}

void mmi_free_bootarg_res(void)
{
	kfree(bootargs_str);
	bootargs_str = NULL;
}

void get_moto_config_file_name(char* name, WIFI_CFG_ENUM index)
{
	char device[ARRAY_VALUE_MAX] = { 0 };
	char radio[ARRAY_VALUE_MAX] = { 0 };
	int num = 0;
	int i = 0;
	char *s;

	if (mmi_get_bootarg("androidboot.device=", &s) == 0) {
		memcpy(device, s, strlen(s));
		DBGLOG(RLM, INFO, "[MOTO]bootargs get device: %s\n", device);
		mmi_free_bootarg_res();
	}
	if (mmi_get_bootarg("androidboot.radio=", &s) == 0) {
		memcpy(radio, s, strlen(s));
		DBGLOG(RLM, INFO, "[MOTO]bootargs get radio: %s\n", radio);
		mmi_free_bootarg_res();
	}

    num = sizeof(products_list) / sizeof(moto_product);
    for (i = 0; i < num; i++) {
        if (strncmp(device, (products_list + i)->hw_device, ARRAY_VALUE_MAX) == 0) {
            if (strncmp(radio, (products_list + i)->hw_radio, ARRAY_VALUE_MAX) == 0 ||
                strncmp((products_list + i)->hw_radio, "all", ARRAY_VALUE_MAX) == 0) {
                if (index == WIFI_CFG_INDEX) {
                    snprintf(name, ARRAY_VALUE_MAX, "%s.cfg", (products_list + i)->wifi_cfg_name);
                }
                else {
                    snprintf(name, ARRAY_VALUE_MAX, "%s.cfg", (products_list + i)->txpowerctrl_name);
                }
                DBGLOG(RLM, ERROR, "[MOTO]Use moto config file name: %s\n", name);
                return;
            }
        }
    }

}
// END IKSWR-130356
