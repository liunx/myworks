/**
 * Parse a configure file
 * copy from vsftpd parseconfig.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int enable_1, enable_2, enable_3, enable_4;

static struct parseconf_bool_setting {
	const char *p_setting_name;
	int *p_variable;
}
parseconf_bool_array[] = {
	{"enable_1", &enable_1},
	{"enable_2", &enable_2},
	{"enable_3", &enable_3},
	{"enable_4", &enable_4},
	// The last one show the end
	{0, 0}
};

unsigned int num_1, num_2, num_3, num_4;

static struct parseconf_uint_setting {
	const char *p_setting_name;
	unsigned int *p_variable;
}
parseconf_uint_array[] = {
	{"num_1", &num_1},
	{"num_2", &num_2},
	{"num_3", &num_3},
	{"num_4", &num_4},
	{0, 0}
};

char *name_1, *name_2, *name_3, *name_4;

static struct parseconf_str_setting {
	const char *p_setting_name;
	const char **p_variable;
}
parseconf_str_array[] = {
	{"name_1", &name_1},
	{"name_2", &name_2},
	{"name_3", &name_3},
	{"name_4", &name_4},
	{0, 0}
};

void parseconf_load_setting(const char *p_setting, int errs_fatal)
{

}
