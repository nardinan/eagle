/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cstring.h"
char *cstring::nearchar (const char *string, const char *dividers) {
    char *pointers[3] = {NULL, NULL, NULL};
    pointers[1] = strchr(string, '\0');
    if (dividers) {
        for (pointers[2] = (char *)dividers; *(pointers[2]); (pointers[2])++)
            if ((pointers[0] = strchr(string, *(pointers[2]))) && ((!(pointers[1])) || (pointers[0] < pointers[1])))
                pointers[1] = pointers[0];
    }
    return pointers[1];
}

carray<char> *cstring::explode (const char *string, const char *dividers, int limit) {
    carray<char> *result;
    char *singleton = NULL, *pointers[2] = {NULL, NULL}, *characters = (char *)dividers;
    size_t segments = 0, length;
    length = estrlen(string);
    pointers[0] = (char *)string;
    if ((result = enew carray<char>)) {
        while ((pointers[1] = cstring::nearchar(pointers[0], characters))) {
            if ((pointers[1]-pointers[0]) > 0) {
                if ((singleton = (char *) emalloc((pointers[1]-pointers[0])+1))) {
                    strncpy(singleton, pointers[0], (pointers[1]-pointers[0]));
                    singleton[(pointers[1]-pointers[0])] = '\0';
                    if (!result->add(singleton))
                        break;
                } else
                    ekill("out of memory");
            }
            if ((pointers[1]-string) < length) {
                pointers[0] = (pointers[1]+1);
                if ((limit >= 0) && (limit <= (++segments)))
                    characters = NULL;
            } else
                break;
        }
    } else
        ekill("out of memory");
    return result;
}

int cstring::defrag (const char *string, const char *dividers, const char *format, ...) {
    va_list arguments;
    char integer[EE_DIGITS_SIZE], *pointer = NULL, **stringbackup = NULL;
    unsigned int index, args = 0;
    int *valuebackup = NULL, result = EE_OK;
    size_t length;
    va_start(arguments, format);
    {
        length = estrlen(format);
        for (index = 0; index < length; index++) {
            if ((pointer = cstring::nearchar(string, dividers))) {
                switch(format[index]) {
                    case 's':
                    case 'S':
                        stringbackup = va_arg(arguments, char**);
                        if ((*stringbackup = (char *) emalloc((pointer-string)+1)))
                            memcpy(*stringbackup, string, pointer-string);
                        else
                            ekill("out of memory");
                        break;
                    case 'd':
                    case 'D':
                        valuebackup = va_arg(arguments, int*);
                        memcpy(integer, string, pointer-string);
                        integer[pointer-string] = '\0';
                        *valuebackup = atoi(integer);
                        break;
                    default:
                        result = EE_ERROR;
                }
            } else
                result = EE_ERROR;
            if (!result)
                break;
            args++;
            if ((*pointer != '\0') && (*pointer != '\n'))
                string = (pointer+1);
            else
                break;
        }
    }
    va_end(arguments);
    return (args!=estrlen(format))?EE_ERROR:result;
}

int cstring::trim (char **string) {
    char *pointer[2] = {*string, NULL};
    int result = EE_OK;
    /* fast trim: don't change the size of datablock */
    pointer[1] = (pointer[0]+estrlen(pointer[0]))-1;
    while (((spacechar(*(pointer[0]))) || (finalchar(*(pointer[1]))) || (spacechar(*(pointer[1])))) && (pointer[0] <= pointer[1])) {
        if (spacechar(*(pointer[0])))
            pointer[0]++;
        if ((finalchar(*(pointer[1]))) || (spacechar(*(pointer[1])))) {
            *(pointer[1]) = '\0';
            pointer[1]--;
        }
    }
    if ((pointer[0]) > (*string))
        memmove(*string, pointer[0], (estrlen(pointer[0])+1));
    return result;
}

int cstring::compress (char **string, const char *dictionary) {
    unsigned int index[3] = {0, 0, 0};
    size_t length[2] = {estrlen(*string), estrlen(dictionary)};
    int result = EE_OK;
    if ((length[1] <= EE_DICTIONARY_SIZE) && ((length[1]%2) == 0)) {
        for (index[0] = 0; index[0] < length[0]; index[0]++) {
            for (index[1] = 0; (index[1] < length[1]); index[1] += 2)
                if ((dictionary[index[1]] == (*string)[index[0]]) && (dictionary[index[1]+1]==(*string)[index[0]+1]))
                    break;
            if (index[1] < length[1]) {
                (*string)[index[2]++] = (char)((index[1]/2)+(EE_ASCII_MAX-EE_ASCII_MIN));
                index[0]++;
            } else
                (*string)[index[2]++] = (char)(((*string)[index[0]])-EE_ASCII_MIN);
        }
        (*string)[index[2]] = '\0';
        if (!(*string = (char *) erealloc(*string, index[2]+1)))
            ekill("out of memory");
    } else
        result = EE_ERROR;
    return result;
}

int cstring::decompress (char **string, const char *dictionary) {
    unsigned int index[2] = {0, 0};
    size_t length[2] = {estrlen(*string), estrlen(dictionary)};
    char *pointer = NULL;
    int result = EE_OK;
    if ((length[1] <= EE_DICTIONARY_SIZE) && ((length[1]%2) == 0)) {
        if ((pointer = (char *) emalloc((length[0]*2)+1))) {
            for (index[0] = 0; index[0] < length[0]; index[0]++) {
                if (((*string)[index[0]] < (EE_ASCII_MAX-EE_ASCII_MIN)))
                    pointer[index[1]++] = (char)((*string)[index[0]]+EE_ASCII_MIN);
                else {
                    pointer[index[1]++] = dictionary[(((*string)[index[0]]-(EE_ASCII_MAX-EE_ASCII_MIN))*2)];
                    pointer[index[1]++] = dictionary[(((*string)[index[0]]-(EE_ASCII_MAX-EE_ASCII_MIN))*2)+1];
                }
            }
            pointer[index[1]] = '\0';
            efree(*string);
            if ((*string = (char *) emalloc(index[1]+1))) {
                strcpy(*string, pointer);
                *string[index[1]] = '\0';
            } else
                ekill("out of memory");
            efree(pointer);
        } else
            ekill("out of memory");
    } else
        result = EE_ERROR;
    return result;
}
