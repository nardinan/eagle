/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cfilesystem.h"
int cfilesystem::initialize (sdfssegment *datablock) {
	sdfssegment *segment = NULL;
    sdfsheader mainheader;
	int result = EE_OK;
    unsigned int records;
    unsigned char *pointer = datablock->datablock;
    if (memcpy(&mainheader, pointer, sizeof(sdfsheader))) {
        if (estrcmp(mainheader.label, EE_DFS) == 0) {
            if (cendian::check() == EE_BIG_ENDIAN)
                eswap(mainheader.size);
            pointer += sizeof(sdfsheader);
            for (records = 0; records < mainheader.size; records++) {
                if ((segment = (sdfssegment *) emalloc(sizeof(sdfssegment)))) {
                    segment->datablock = NULL;
                    if (memcpy(&segment->header, pointer, sizeof(sdfsheader))) {
                        if (cendian::check() == EE_BIG_ENDIAN)
                            eswap(segment->header.size);
                        pointer += sizeof(sdfsheader);
                        if ((segment->datablock = (unsigned char *) emalloc(segment->header.size))) {
                            if (memcpy(segment->datablock, pointer, segment->header.size)) {
                                result = segments.add(segment, segment->header.label);
                                pointer += segment->header.size;
                            } else
                                result = EE_ERROR;
                        } else
                            ekill("out of memory");
                    } else
                        result = EE_ERROR;
                    if (!result) {
                        if (segment->datablock)
                            efree(segment->datablock);
                        efree(segment);
                        break;
                    }
                } else
                    ekill("out of memory");
            }
        } else
            result = EE_ERROR;
    } else
        result = EE_ERROR;
     return result;
}

int cfilesystem::initialize (const char *filename) {
    FILE *filestream = NULL;
	sdfssegment *segment = NULL;
    sdfsheader mainheader;
	int result = EE_OK;
    unsigned int records;
	if ((filestream = fopen(filename, "rb"))) {
        if (fread(&mainheader, sizeof(sdfsheader), 1, filestream)) {
            if (estrcmp(mainheader.label, EE_DFS) == 0) {
                if (cendian::check() == EE_BIG_ENDIAN)
                    eswap(mainheader.size);
                for (records = 0; records < mainheader.size; records++) {
                    if ((segment = (sdfssegment *) emalloc(sizeof(sdfssegment)))) {
                        segment->datablock = NULL;
                        if (fread(&segment->header, sizeof(sdfsheader), 1, filestream)) {
                            if (cendian::check() == EE_BIG_ENDIAN)
                                eswap(segment->header.size);
                            if ((segment->datablock = (unsigned char *) emalloc(segment->header.size))) {
                                if (fread(segment->datablock, segment->header.size, 1, filestream))
                                    result = segments.add(segment, segment->header.label);
                                else
                                    result = EE_ERROR;
                            } else
                                ekill("out of memory");
                        } else
                            result = EE_ERROR;
                        if (!result) {
                            if (segment->datablock)
                                efree(segment->datablock);
                            efree(segment);
                            break;
                        }
                    } else
                        ekill("out of memory");
                }
            } else
                result = EE_ERROR;
        } else
            result = EE_ERROR;
		fclose(filestream);
	} else
        result = EE_ERROR;
	return result;
}

int cfilesystem::add (const char *filename, const char *label) {
    FILE *filestream = NULL;
	sdfssegment *segment = NULL;
	int result = EE_OK;
	if ((filestream = fopen(filename, "rb"))) {
		if ((segment = (sdfssegment *) emalloc(sizeof(sdfssegment)))) {
			strncpy(segment->header.label, label, EE_LABEL_SIZE);
            segment->header.label[estrlen(label)] = '\0';
			fseek(filestream, 0, SEEK_END);
			segment->header.size = ftell(filestream);
            if ((segment->datablock = (unsigned char *) emalloc(segment->header.size))) {
                fseek(filestream, 0, SEEK_SET);
                if (fread(segment->datablock, segment->header.size, 1, filestream))
                    result = segments.add(segment, label);
                else
                    result = EE_ERROR;
            } else
                ekill("out of memory");
		} else
            ekill("out of memory");
		fclose(filestream);
	} else
        result = EE_ERROR;
	return result;
}

int cfilesystem::del (const char *label) {
    sdfssegment *segment = NULL;
	int result = EE_OK;
	if ((segment = segments.get(label))) {
		if (segment->datablock)
            efree(segment->datablock);
		efree(segment);
        segments.del();
	} else
        result = EE_ERROR;
	return result;
}

int cfilesystem::collapse (const char *filename) {
    FILE *filestream = NULL;
	sdfssegment *segment = NULL;
    sdfsheader mainheader;
    size_t size;
	int result = EE_OK;
	if ((filestream = fopen(filename, "wb"))) {
        strcpy(mainheader.label, EE_DFS);
        mainheader.label[estrlen(EE_DFS)] = '\0';
        mainheader.size = segments.records();
        if (cendian::check() == EE_BIG_ENDIAN)
            eswap(mainheader.size);
        if (fwrite(&mainheader, sizeof(sdfsheader), 1, filestream)) {
            foreach(segment, &segments) {
                size = segment->header.size;
                if (cendian::check() == EE_BIG_ENDIAN)
                    eswap(segment->header.size);
                if (fwrite(&segment->header, sizeof(sdfsheader), 1, filestream)) {
                    if (!fwrite(segment->datablock, size, 1, filestream))
                        result = EE_ERROR;
                } else
                    result = EE_ERROR;
                segment->header.size = size;
                if (!result)
                    break;
            }
        } else
            result = EE_ERROR;
		fclose(filestream);
	} else
        result = EE_ERROR;
	return result;
}

void cfilesystem::unload (sdfssegment *singleton) {
    if (singleton->datablock)
        efree(singleton->datablock);
    efree(singleton);
}

void cfilesystem::unload (void) {
    destroy(&segments, this->unload, sdfssegment);
    this->init();
}
