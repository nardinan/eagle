/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cxml.h"
void cxml::init (void) {
    if ((structure = (sxmlclass *) emalloc(sizeof(sxmlclass)))) {
        memset(structure->label, '\0', (sizeof(char)*EE_LABEL_SIZE));
        structure->value = NULL;
        structure->subclass.init();
        structure->keys.init();
        structure->back = pointer = structure;
    } else
        ekill("out of memory");
}

int cxml::analyze (sdfssegment *datablock) {
    carray<sxmltoken> *tokens = NULL;
    int result = EE_OK;
    if ((tokens = cxml::xmltokenizer(datablock))) {
        result = analyze(tokens);
        destroy(tokens, efree, sxmltoken);
        delete(tokens);
    } else
        result = EE_ERROR;
    return result;
}

int cxml::analyze (carray<sxmltoken> *tokens) {
    sxmltoken *element, *back = NULL;
    exmlaction action = EXMLACTION_READ;
    int result = EE_OK;
    bool passed;
    foreach(element, tokens) {
        passed = false;
        switch (element->token[0]) {
            case '<':
                if (element->length > 1) {
                    if ((element->token[1] == '/') || (element->token[1] == '?')) {
                        action = EXMLACTION_JUMP;
                        passed = true;
                    }
                } else {
                    action = EXMLACTION_TITLE;
                    passed = true;
                }
                break;
            case '/':
                if (element->length > 1) {
                    if (element->token[1] == '>') {
                        pointer = pointer->back;
                        action = EXMLACTION_CVALUE;
                        passed = true;
                    }
                }
                break;
            case '>':
                if (action == EXMLACTION_JUMP)
                    pointer = pointer->back;
                action = EXMLACTION_CVALUE;
                passed = true;
                break;
        }
        if ((!passed) && (action != EXMLACTION_JUMP)) {
            switch (action) {
                case EXMLACTION_TITLE:
                    if (!this->addclass(element->token))
                        result = EE_ERROR;
                    action = EXMLACTION_KEY;
                    break;
                case EXMLACTION_KEY:
                    back = element;
                    action = EXMLACTION_KVALUE;
                    break;
                case EXMLACTION_KVALUE:
                    if (estrcmp(element->token, "=") != 0) {
                        if (!this->addkey(element->token, back->token))
                            result = EE_ERROR;
                        action = EXMLACTION_KEY;
                    }
                    break;
                case EXMLACTION_CVALUE:
                    if (!this->addvalue(element->token))
                        result = EE_ERROR;
                    break;
                default:
                    break;
            }
        }
        if (!result)
            break;
    }
    return result;
}

int cxml::draw (FILE *stream, sxmlclass *value, int deep) {
    char *back;
    int index, result = EE_OK;
    sxmlclass *item;
    if (value) {
        if (estrlen(value->label) > 0) {
            for (index = 0; index < deep; index++)
                fprintf(stream, "\t");
            fprintf(stream, "<%s", value->label);
            foreach(back, &value->keys)
                fprintf(stream, " %s = %s", value->keys.label(), back);
            fprintf(stream, ">");
        }
        if (value->value)
            fprintf(stream, "%s", value->value);
        if (value->subclass.records() > 0)
            fprintf(stream, "\n");
        foreach(item, &value->subclass)
            draw(stream, item, deep+1);
        if (estrlen(value->label) > 0) {
            if (value->subclass.records() > 0)
                for (index = 0; index < deep; index++)
                    fprintf(stream, "\t");
            fprintf(stream, "</%s>\n", value->label);
        }
    }
    return result;
}

int cxml::addkey (const char *value, const char *label) {
    char *buffer = NULL;
    int result = EE_OK;
    if (pointer) {
        if ((buffer = (char *) emalloc(strlen(value)+1))) {
            strcpy(buffer, value);
            buffer[estrlen(value)] = '\0';
            if (!(result = pointer->keys.add(buffer, label)))
                efree(buffer);
        } else
            ekill("out of memory");
    }
    return result;
}

int cxml::addvalue (const char *value) {
    size_t total;
    int result = EE_OK;
    if (pointer) {
        total = (estrlen(pointer->value)+estrlen(value));
        strapp(pointer->value, value);
        if (strlen(pointer->value) != total)
            result = EE_ERROR;
    }
    return result;
}

int cxml::addclass (const char *value) {
    sxmlclass *backup = NULL;
    int result = EE_OK;
    if (pointer) {
        if ((backup = (sxmlclass *) emalloc(sizeof(sxmlclass)))) {
            memset(backup->label, '\0', (sizeof(char)*EE_LABEL_SIZE));
            strncpy(backup->label, value, (EE_LABEL_SIZE-1));
            backup->value = NULL;
            backup->back = pointer;
            backup->subclass.init();
            backup->keys.init();
            if ((pointer->subclass.add(backup, value)))
                pointer = backup;
            else {
                efree(backup);
                result = EE_ERROR;
            }
        } else
            ekill("out of memory");
    }
    return result;
}

sxmlclass *cxml::get (const char *format, ...) {
    va_list args;
    sxmlclass *result = structure;
    if (format) {
        va_start(args, format);
        {
            result = vaget(format, args);
        }
        va_end(args);
    }
    return result;
}

sxmlclass *cxml::vaget (const char *format, va_list args) {
    size_t length = estrlen(format), index;
    sxmlclass *result = structure;
    for (index = 0; index < length; index++)
        if ((!result) || (!(result = result->subclass.get(va_arg(args, char*)))))
            break;
    return result;
}

carray<sxmltoken> *cxml::xmltokenizer (sdfssegment *datablock) {
    char *destination = (char *)datablock->datablock;
    carray<sxmltoken> *result = NULL;
    sxmltoken *singleton = NULL;
    sxmlflags flags[2]; /* 0-current; 1-last */
    sxmlcontroller controller;
    int index;
    bool consistence;
    if ((destination) && (result = enew carray<sxmltoken>())) {
        flagsassign(flags, 2, false);
        controller.opened = controller.closed = false;
        while (*destination) {
            if (constchar(*destination)) {
                switch (*destination) {
                    case '?':
                    case '/':
                    case '=':
                        if ((controller.opened)) {
                            flags[0].symbols = true;
                            flags[0].characters = flags[0].safe = false;
                        } else {
                            flags[0].characters = true;
                            flags[0].symbols = flags[0].safe = false;
                        }
                        break;
                    case '<':
                        controller.opened = true;
                        controller.closed = false;
                        flags[0].symbols = true;
                        flags[0].characters = flags[0].safe = false;
                        break;
                    case '>':
                        controller.closed = true;
                        controller.opened = false;
                        flags[0].symbols = true;
                        flags[0].characters = flags[0].safe = false;
                        break;
                    case '\\':
                        flags[0].safe = flags[0].special = true;
                        break;
                    case ' ':
                        /* ignoring space if is not a value */
                        if (controller.closed)
                            break;
                    case '\t':
                        flags[0].special = true;
                        flags[0].symbols = flags[0].characters = flags[0].safe = false;
                        break;
                    default:
                        flags[0].symbols = flags[0].safe = false;
                        flags[0].characters = true;
                }
                if (singleton) {
                    flagscheck(flags,2,consistence);
                    if (consistence)
                        singleton->token[singleton->length++] = *destination;
                    else {
                        index = 0;
                        while (index < singleton->length) {
                            if (realmchar(singleton->token[index])) {
                                consistence = true;
                                break;
                            }
                            index++;
                        }
                        if (consistence) {
                            if (!result->add(singleton))
                                efree(singleton);                           
                        } else
                            efree(singleton);
                        singleton = NULL;
                    }
                }
                if (!singleton) {
                    if ((singleton = (sxmltoken *) emalloc(sizeof(sxmltoken)))) {
                        memset(singleton->token, '\0', (sizeof(char)*EE_TOKEN_SIZE));
                        singleton->length = 0;
                        singleton->token[singleton->length++] = *destination;
                        flags[1].symbols = flags[0].symbols;
                        flags[1].special = flags[0].special;
                        flags[1].characters = flags[0].characters;
                    } else
                        ekill("out of memory");
                }
            }
            destination++;
        }
        if (singleton) {
            index = 0;
            while (index < singleton->length) {
                if (realmchar(singleton->token[index])) {
                    consistence = true;
                    break;
                }
                index++;
            }
            if (consistence) {
                if (!result->add(singleton))
                    efree(singleton);
            } else
                efree(singleton);
        }
    } else
        ekill("out of memory");
    return result;
}

void cxml::unload (sxmlclass *value) {
    destroy((&(value->subclass)), this->unload, sxmlclass);
    destroy((&(value->keys)), efree, char);
    if (value->value) {
        efree(value->value);
        value->value = NULL;
    }
    efree(value);
}
