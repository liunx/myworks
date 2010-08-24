#ifndef PARSECONF_H_INCLUDED
#define PARSECONF_H_INCLUDED
#include "config.h"
char **str_split_char(char *str, const char *split);
void load_setting(const char *p_setting);

#endif // PARSECONF_H_INCLUDED
