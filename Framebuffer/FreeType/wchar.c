#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() 
{
    wchar_t str[] = L"中文aha";
    for(int i=0; i<wcslen(str); i++)
    {
        printf("%02x\n", str[i]);
    }
}