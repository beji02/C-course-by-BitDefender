#include "common.h"
#include "ccstring.h"


void Strcpy(char* Destination, char* Source)
{
    if (Destination == NULL || Source == NULL)
        return;

    while (*Source != 0)
    {
        *Destination = *Source;
        Destination++;
        Source++;
    }
    *Destination = 0;
}
int Strcmp(const char* S1, const char* S2)
{
    if (S1 == NULL || S2 == NULL)
        return -1;
    while (*S1 == *S2 && *S1 != 0 && *S2 != 0)
    {
        S1++;
        S2++;
    }
    if (*S1 == 0 && *S2 != 0)
        return -1;
    if (*S2 == 0 && *S1 != 0)
        return 1;
    return *S1 - *S2;
}
int Strlen(const char* S)
{
    if (S == NULL)
        return -1;
    int k = 0;
    while (*S != 0)
        S++, k++;
    return k;
}