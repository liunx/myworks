#ifndef PARSECONFIG_H
#define PARSECONFIG_H

char **str_split_char(char *str, const char *split);
void get_word(char *line);
void get_number(char *line);
void get_comment(char *line);
char *clean_space(char *line);
char *clean_comment(char *line);
char *read_file(const char *filename);

#endif
