#ifndef macros_h
#define macros_h

#define CAT(a, b) a##b
#define XCAT(a, b) CAT(a, b)

#define STR(s) #s
#define XSTR(s) STR(s)

#define CREATE_PATH(path, file) path "/" file

#define UNUSED(x) (void)(x)

#endif