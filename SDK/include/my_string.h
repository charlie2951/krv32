#ifndef MY_STRING_H
#define MY_STRING_H

int my_strcmp(const char *str, const char *prefix);
unsigned int my_strlen(const char *s);
unsigned int my_int2str(int val, char *buf);
char* my_strcat(char *dest, const char *src);
char* my_strncat(char *dest, const char *src, unsigned int n);
void* my_memcpy(void *dest, const void *src, unsigned int n);
void* my_memset(void *s, int c, unsigned int n);
void my_sprintf(char *buf, const char *arg1, const char *arg2);
#endif

