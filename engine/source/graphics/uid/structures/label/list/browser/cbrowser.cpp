/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cbrowser.h"
#ifndef _WIN32
/* unix style */
char pathcharacter = '/';
char pathseparator[] = "/";
char pathroot[] = "/";
#else
/* windows style */
char pathcharacter = '\\';
char pathseparator[] = "\\";
char pathroot[] = "C:\\";
#endif
efilter& operator++ (efilter& value) { return value = (value==EFILTER_NULL)?EFILTER_SPECIALS:efilter(value+1); }
void cbrowser::init (const char *startpoint) {
    int index = 0;
    clist::init(); /* father's init */
    memset(choosed, '\0', (sizeof(char)*EE_LABEL_SIZE));
    memset(abspath, '\0', (sizeof(char)*EE_BROWSER_SIZE));
    snprintf(abspath, EE_BROWSER_SIZE, "%s", startpoint);
    handler = NULL;
    background = EE_BROWSER_BACKGROUND;
    selected = false;
    for (index = 0; index < EFILTER_NULL; index++)
        filter[index] = false;
    this->refresh();
}

void cbrowser::content (const char *value) {
    bool newline = true;
    scharacter *singleton;
    sfont *icons;
    clist::content(value); /* father's init */
    if ((icons = clabel::replacement->get("icons"))) {
        foreach(singleton, &characters) {
            if (newline) {
                switch (singleton->character) {
                    case '6':
                    case '9':
                    case '2':
                    case '.':
                    case ',':
                        singleton->font = icons;
                        break;
                }
            }
            newline = false;
            if (singleton->character == '\n')
                newline = true;
        }
    }
}

void cbrowser::handleizer (const char *identifier, fbrowserhandler *handler) {
    strncpy(this->identifier, identifier, EE_LABEL_SIZE);
    this->identifier[EE_MIN(estrlen(identifier), (EE_LABEL_SIZE-1))] = '\0';
    this->handler = handler;
}

void cbrowser::forward (skeyboard *keyboard, smouse *mouse, unsigned int positionx, unsigned int positiony) {
    bool present;
    int absolute[EPOSITION_NULL], result, index = 0;
    carray<char> *elements[2];
    if (mouse) {
        absolute[EPOSITION_POSITIONX] = mouse->positionx;
        absolute[EPOSITION_POSITIONY] = mouse->positiony;
        mouse->positionx = positionx;
        mouse->positiony = positiony;
        inrange(mouse, destination(EPOSITION_POSITIONX), destination(EPOSITION_POSITIONY), destination(EPOSITION_WIDTH), destination(EPOSITION_HEIGHT), present);
        if (present) {
            if ((mouse->action == EACTION_RELEASED) && (mouse->key == EMOUSE_LEFT)) {
                if ((result = search(mouse->positionx, mouse->positiony)) >= 0) {
                    for (;result < characters.records(); result++) {
                        if (characters.get(result)->character == '\n')
                            break;
                        choosed[index++] = characters.get(result)->character;
                    }
                    choosed[index] = '\0';
                    if ((elements[0] = cstring::explode(choosed, " ", 1))) { 
                        if (estrcmp(elements[0]->get((size_t)0), "[BACK]") == 0) {
                            if ((elements[1] = cstring::explode(abspath, pathseparator))) {
                                memset(abspath, '\0', (sizeof(char)*strlen(abspath)));
                                if (elements[1]->records() <= 1)
                                    snprintf(abspath, EE_BROWSER_SIZE, "%s", pathroot);
                                else
                                    for (result = 0; result < (elements[1]->records()-1); result++) {
                                        if (abspath[(estrlen(abspath)-1)] == pathcharacter)
                                            abspath[(estrlen(abspath)-1)] = '\0';
                                        snprintf(abspath, EE_BROWSER_SIZE, "%s%s%s", abspath, pathseparator, elements[1]->get((unsigned int)result));
                                    }
                                destroy(elements[1], efree, char);
                                delete(elements[1]);
                            }
                        } else if (estrcmp(elements[0]->get((size_t)0), "6") == 0) {
                            if (estrcmp(abspath, pathroot) == 0)
                               memset(abspath, '\0', (sizeof(char)*strlen(abspath)));
                            if (abspath[(estrlen(abspath)-1)] == pathcharacter)
                                abspath[(estrlen(abspath)-1)] = '\0';
                            snprintf(abspath, EE_BROWSER_SIZE, "%s%s%s", abspath, pathseparator, elements[0]->get((unsigned int)1));
                        }
                        if (handler)
                            handler(this, identifier, abspath, choosed);
                        this->refresh();
                    }
                    destroy(elements[0], efree, char);
                    delete(elements[0]);
                }
            }
        }
        mouse->positionx = absolute[EPOSITION_POSITIONX];
        mouse->positiony = absolute[EPOSITION_POSITIONY];
    }
}

char *cbrowser::content (void) {
    char *result = NULL;
    if ((result = (char *) emalloc(estrlen(abspath)+1))) {
        strcpy(result, abspath);
        result[estrlen(abspath)] = '\0';
    } else
        ekill("out of memory");
    return result;
}

void cbrowser::refresh (void) {
    DIR *directory;
#ifdef _WIN32
    DIR *hook;
#endif
    struct dirent *point;
    struct stat informations;
    char *content = NULL, singleton[EE_BROWSER_SIZE];
    bool skip = false;
    if ((directory = opendir(abspath))) {
        strapp(content, "[BACK]\n");
        while ((point = readdir(directory))) {
            skip = false;
            if (filter[EFILTER_HIDDENS])
                if (point->d_name[0] == '.')
                    skip = true;
            snprintf(singleton, EE_BROWSER_SIZE, "%s%s%s", abspath, pathseparator, point->d_name);
            /*
                9 is a file
                6 is a directory
                2 is a link
                . is a device
               , is a file of another kind
             */
#ifndef _WIN32
            if (lstat(singleton, &informations) == 0) {
                if (S_ISREG(informations.st_mode)) {
                    if (filter[EFILTER_FILES])
                        skip = true;
                    snprintf(singleton, EE_LABEL_SIZE, "9");
                } else if (S_ISLNK(informations.st_mode)) {
                    if (filter[EFILTER_LINKS])
                        skip = true;
                    snprintf(singleton, EE_LABEL_SIZE, "2");
                } else if (S_ISDIR(informations.st_mode)) {
                    if (filter[EFILTER_DIRECTORIES])
                        skip = true;
                    snprintf(singleton, EE_LABEL_SIZE, "6");
                } else {
                    if (filter[EFILTER_SPECIALS])
                        skip = true;
                    snprintf(singleton, EE_LABEL_SIZE, ",");
                }
            }
#else
            if ((hook = opendir(singleton))) {
                if (filter[EFILTER_DIRECTORIES])
                    skip = true;
                snprintf(singleton, EE_LABEL_SIZE, "6");
                closedir(hook);
            } else {
                if (filter[EFILTER_FILES])
                    skip = true;
                snprintf(singleton, EE_LABEL_SIZE, "9");
            }
#endif
            snprintf(singleton, EE_LABEL_SIZE, "%s %s\n", singleton, point->d_name);
            if (!skip)
                strapp(content, singleton);
        }
        this->content(content);
        efree(content);
        closedir(directory);
    }
    this->view(EPOSITION_POSITIONY, 0);
}
