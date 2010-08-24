#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED
#define array_size      64
/**
 * Copy from vsftpd-2.3.0 parseconf.c
 * Author: Cris Evans
 */
extern const char *total_servers;
extern const char *total_clients;

struct parseconf_str_setting {
	const char *p_setting_name;
	const char **p_variable;
};

#endif // CONFIG_H_INCLUDED
