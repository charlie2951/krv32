#include "my_string.h"
#include <stdarg.h>
// --- Your Library Functions ---

char* my_strstr(const char *haystack, const char *needle) {
    if (*needle == '\0') return (char *)haystack;

    for (; *haystack != '\0'; haystack++) {
        if (*haystack == *needle) {
            const char *h = haystack;
            const char *n = needle;
            
            // Use your existing my_strcmp logic here
            // If my_strcmp returns 1, it means the prefix matches
            if (my_strcmp(h, n)) {
                return (char *)haystack;
            }
        }
    }
    return 0; // Return NULL if not found
}

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
/*
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
*/
unsigned int my_int2str(int val, char *buf) {
    char tmp[11];
    unsigned int i = 0, j;
    
    if (val == 0) { *buf = '0'; return 1; }
    
    unsigned int n = val;
    if (val < 0) {
        *buf++ = '-';
        n = -val;
    }

    while (n > 0) {
        tmp[i++] = (n % 10) + '0';
        n /= 10;
    }
    for (j = 0; j < i; j++) buf[j] = tmp[i - j - 1];
    
    // Note: Don't null terminate here if using inside sprintf, 
    // or return length to let sprintf handle it.
    return i + (val < 0 ? 1 : 0); 
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
char* my_strcpy(char *dest, const char *src) {
    char *ptr = dest;
    while ((*ptr++ = *src++) != '\0');
    return dest;
}

// Helper to convert unsigned int to string with a specific base
unsigned int my_uint2str(unsigned int val, char *buf, int base) {
    char tmp[33];
    char *digits = "0123456789abcdef";
    unsigned int i = 0, j = 0;

    if (val == 0) {
        buf[0] = '0';
        return 1;
    }

    while (val > 0) {
        tmp[i++] = digits[val % base];
        val /= base;
    }

    // Reverse into the destination buffer
    for (j = 0; j < i; j++) {
        buf[j] = tmp[i - j - 1];
    }
    return i;
}
/*
//similar to sprintf()
// Usage:char atcommand[128]; build_connect_cmd(atcommand, "MyHomeWiFi", "SecretPassword123");
void my_sprintf(char *buf, const char *arg1) {
    // 1. Clear the buffer to ensure it's fresh
    my_memset(buf, 0, 128); 

    // 2. Start building the string piece by piece
    my_strcat(buf, arg1);
   // my_strcat(buf,arg2);
    //my_strcat(buf, "\"");
}
    */

    //Revised sprintf() to accept %d or %s or %x specifier
    void my_sprintf(char *buf, const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *p_buf = buf;

    while (*format != '\0') {
        if (*format == '%') {
            format++; // Move to specifier
            switch (*format) {
                case 's': {
                    char *s = va_arg(args, char *);
                    if (!s) s = "(null)";
                    while (*s) *p_buf++ = *s++;
                    break;
                }
                case 'd': {
                    int val = va_arg(args, int);
                    // Use the existing int2str (assuming it returns length)
                    p_buf += my_int2str(val, p_buf);
                    break;
                }
                case 'x': {
                    unsigned int val = va_arg(args, unsigned int);
                    // Add 0x prefix if you like, or leave it for the format string
                    p_buf += my_uint2str(val, p_buf, 16);
                    break;
                }
                case '%': {
                    *p_buf++ = '%';
                    break;
                }
                default: {
                    *p_buf++ = '%';
                    *p_buf++ = *format;
                    break;
                }
            }
        } else {
            *p_buf++ = *format;
        }
        format++;
    }

    *p_buf = '\0'; // Final Null Terminator
    va_end(args);
}
