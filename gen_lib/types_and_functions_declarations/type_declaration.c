
#include "type_declaration.h"

void * nonConstMemcpyDecl(void * dest, void * src, size_t n)
{
    return memcpy(dest, (const void *)src, n);
}