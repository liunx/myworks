#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "parseconf.h"

const char *total_servers;
const char *total_clients;

struct parseconf_str_setting parseconf_str_array[] = {
	{"Servers", &total_servers},
	{"Clients", &total_clients},
	{0, 0}
};

// str_split_char -- a non-reentry function
// remember to release it when it's useless
char **str_split_char(char *str, const char *split)
{
	char **pptr = NULL;
	char *pstr;
	char *savestr;
	char *token;
	int i;
	pptr = (char **)malloc(sizeof(char **) * array_size);
	if (pptr == NULL) {
		fprintf(stderr, "Failed to allocate the room.\n");
		return NULL;
	}
	memset(pptr, 0, sizeof(char **) * array_size);

	for (pstr = str, i = 0; ; i++, pstr = NULL) {
		token = strtok_r(pstr, split, &savestr);
		if (token == NULL)
			break;
		pptr[i] = token;
	}
	pptr[i] = NULL;

	return pptr;

}

void load_setting(const char *p_setting)
{
	char **ptr = NULL;
	const struct parseconf_str_setting *p_str_setting;
	p_str_setting = parseconf_str_array;
	int retval;

	// We get the input string in a syle of  x = xxx,
	// we need split them out
	while (p_str_setting->p_setting_name != 0) {
		ptr = str_split_char(p_setting, "=");
		if (ptr == NULL) {
			fprintf(stderr, "Failed to parse the line.\n");
			exit(1);
		}
		retval = strcmp(p_str_setting->p_setting_name, ptr[0]);
		if (retval == 0) {
			const char **p_curr_setting = p_str_setting->p_variable;
			// We should do a check that if the pointer is not null,
			// we should release the old one.
			if (*p_curr_setting != NULL) {
				free((char *) *p_curr_setting);
			}

			if (ptr[1] == NULL) {
				*p_curr_setting = NULL;
			} else {

				*p_curr_setting = strdup(ptr[1]);
			}

			free(ptr);
			return;
		}

		p_str_setting++;

	}
	// Do not forget free the room when it useless
	free(ptr);

}

