#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "parseconf.h"

/**
 * Copy from vsftpd-2.3.0 parseconf.c
 * Author: Cris Evans
 */
char *total_servers = NULL;
char *total_clients = NULL;

static struct parseconf_str_setting {
	const char *p_setting_name;
	const char **p_variable;
}
parseconf_str_array[] = {
	{"Servers", &total_servers},
	{"Clients", &total_clients},
	{0, 0}
};

void str_split_char(const char *str, const char *val, const char *split)
{
	char *token;
	char *savevalue;
	token = strtok_r(str, split, &savevalue);
	val = strtok_r(NULL, split, &savevalue);

}

void load_setting(const char *p_setting)
{
	const struct parseconf_str_setting *p_str_setting;
	p_str_setting = parseconf_str_array;

	// We get the input string in a syle of  x = xxx, 
	// we need split them out

	while (p_str_setting->p_setting_name != 0) {
		// if strstr not null, means match
		if (strstr(p_setting, p_str_setting->p_setting_name)) {
			str_split_char(p_setting, *(p_str_setting->p_variable), "=");

		}

	}

}

int main()
{
	const buf[] = "Servers=localhost:60000";
	load_setting(buf);

	return 0;

}
