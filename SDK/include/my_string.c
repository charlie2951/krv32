#include "my_string.h"
// --- Your Library Functions ---

int my_strcmp(const char *str, const char *prefix) {
    while (*prefix) {
        if (*str++ != *prefix++) return 0;
    }
    return 1;
}

unsigned int my_strlen(const char *s) {
    unsigned int len = 0;
    while (*s++ != '\0') len++;
    return len;
}

unsigned int my_int2str(int val, char *buf) {
    char tmp[11];
    unsigned int i = 0, j;
    if (val == 0) { buf[0] = '0'; buf[1] = '\0'; return 1; }
    while (val > 0) {
        tmp[i++] = (val % 10) + '0';
        val /= 10;
    }
    for (j = 0; j < i; j++) buf[j] = tmp[i - j - 1];
    buf[i] = '\0';
    return i;
}

char* my_strcat(char *dest, const char *src) {
    char *ptr = dest;
    while (*ptr != '\0') ptr++;
    while (*src != '\0') *ptr++ = *src++;
    *ptr = '\0';
    return dest;
}

char* my_strncat(char *dest, const char *src, unsigned int n) {
    char *ptr = dest;
    while (*ptr != '\0') ptr++;
    while (n > 0 && *src != '\0') {
        *ptr++ = *src++;
        n--;
    }
    *ptr = '\0';
    return dest;
}
//memcpy() implementation
void* my_memcpy(void *dest, const void *src, unsigned int n)
{
    // We cast to char* because a char is exactly 1 byte
    char *d = (char *)dest;
    const char *s = (const char *)src;

    while (n--) {
        *d++ = *s++;
    }

    return dest;
}

//memset() implementation
void* my_memset(void *s, int c, unsigned int n) {
    unsigned char *p = (unsigned char *)s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

//similar to sprintf()
// Usage:char atcommand[128]; build_connect_cmd(atcommand, "MyHomeWiFi", "SecretPassword123");
void my_sprintf(char *buf, const char *arg1, const char *arg2) {
    // 1. Clear the buffer to ensure it's fresh
    //my_memset(buf, 0, 128); 

    // 2. Start building the string piece by piece
    my_strcat(buf, arg1);
    my_strcat(buf,arg2);
    //my_strcat(buf, "\"");
}
