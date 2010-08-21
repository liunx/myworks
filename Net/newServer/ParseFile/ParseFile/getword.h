#ifndef GETWORD_H_INCLUDED
#define GETWORD_H_INCLUDED
void get_word(char *line);

void get_number(char *line);

void get_comment(char *line);

char *clean_space(char *line);

char *clean_comment(char *line);

#endif // GETWORD_H_INCLUDED
