/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "ctarga.h"
sdfssegment *ctarga::replacement = NULL;
void ctarga::init (void) {
    ccontext::init(); /* father's init */
    data = mask = NULL;
    returned = false;
}

int ctarga::initialize (sdfssegment *datablock) {
    unsigned char *pointer, backup, rle;
    int result = EE_OK, bppmode;
    size_t total = 0, index;
    scolor *pixel = NULL;
    /* warning default replacement */
    if (!datablock) {
        if (ctarga::replacement)
            datablock = ctarga::replacement;
        else
            result = EE_ERROR;
    }
    /* end */
    if (result) {
        pointer = datablock->datablock;
        if (datablock->header.size > sizeof(stgaheader)) {
            memcpy(&header, pointer, sizeof(stgaheader));
            if (cendian::check() == EE_BIG_ENDIAN) {
                eswap(header.positionx);
                eswap(header.positiony);
                eswap(header.width);
                eswap(header.height);
            }
            bppmode = (header.bpp/8);
            total = ((header.height*header.width)*bppmode);
            if ((data = (unsigned char *) emalloc(total))) {
                if ((header.type[ETGATYPE_IMAGE] != 2) && (header.type[ETGATYPE_IMAGE] != 3)) {
                    header.type[ETGATYPE_IMAGE] = 2;
                    pointer += sizeof(stgaheader);
                    if ((pixel = (scolor *) emalloc(sizeof(scolor)))) {
                        for (index = 0; index < total; ) {
                            memcpy(&rle, pointer, sizeof(unsigned char));
                            pointer += sizeof(unsigned char);
                            if (rle < 128) {
                                rle++;
                                while (rle-- > 0) {
                                    memcpy(pixel, pointer, bppmode);
                                    pointer += bppmode;
                                    data[R(index)] = pixel->red;
                                    data[G(index)] = pixel->green;
                                    data[B(index)] = pixel->blue;
                                    if (bppmode > 3)
                                        data[A(index)] = pixel->alpha;
                                    index += bppmode;
                                }
                            } else {
                                rle -= 127;
                                memcpy(pixel, pointer, bppmode);
                                pointer += bppmode;
                                while (rle-- > 0) {
                                    data[R(index)] = pixel->red;
                                    data[G(index)] = pixel->green;
                                    data[B(index)] = pixel->blue;
                                    if (bppmode > 3)
                                        data[A(index)] = pixel->alpha;
                                    index += bppmode; 
                                }
                            }
                        }
                    } else
                        ekill("out of memory");
                    efree(pixel);
                } else
                    memcpy(data, (pointer+sizeof(stgaheader)+header.oversize), total);
                if (result) {
                    if (bppmode >= 3) { /* flipping bytes */
                        for (index = 0; index < total; index += bppmode) {
                            backup = data[R(index)];
                            data[R(index)] = data[B(index)];
                            data[B(index)] = backup;
                        }
                    }
                    if ((header.description&0x20) == 0x20)
                        flipped[EFLIPDIRECTION_VERTICAL] = true;
                    if ((result = alpha())) {
                        if ((result = texturize())) {
                            crop(EPOSITION_POSITIONX, 0);
                            crop(EPOSITION_POSITIONY, 0);
                            crop(EPOSITION_WIDTH, header.width);
                            crop(EPOSITION_HEIGHT, header.height);
                            drop(EPOSITION_POSITIONX, 0);
                            drop(EPOSITION_POSITIONY, 0);
                            drop(EPOSITION_WIDTH, header.width);
                            drop(EPOSITION_HEIGHT, header.height);
                        } else
                            texture = EE_NULLTEXTURE;
                    }
                }
            } else
                ekill("out of memory");
            pointer += sizeof(stgaheader)+header.oversize;
        } else
            result = EE_ERROR;
    }
    return result;
}

int ctarga::initialize (ctarga *context) {
    int result = EE_OK;
    size_t total;
    header = context->header;
    total = (header.height*header.width);
    rotation = context->rotation;
    flipped[EFLIPDIRECTION_HORIZONTAL] = context->flipped[EFLIPDIRECTION_HORIZONTAL];
    flipped[EFLIPDIRECTION_VERTICAL] = context->flipped[EFLIPDIRECTION_VERTICAL];
    masking(ECOLOR_R, context->gloss(ECOLOR_R));
    masking(ECOLOR_G, context->gloss(ECOLOR_G));
    masking(ECOLOR_B, context->gloss(ECOLOR_B));
    masking(ECOLOR_A, context->gloss(ECOLOR_A));
    if ((data = (unsigned char *) emalloc(total*EE_TARGA_PIXEL))) {
        memcpy(data, context->data, (total*EE_TARGA_PIXEL));
        if ((mask = (unsigned char *) emalloc(total*EE_TARGA_MASK))) {
            memcpy(mask, context->mask, (total*EE_TARGA_MASK));
            if ((result = texturize())) {
                crop(EPOSITION_POSITIONX, 0);
                crop(EPOSITION_POSITIONY, 0);
                crop(EPOSITION_WIDTH, header.width);
                crop(EPOSITION_HEIGHT, header.height);
                drop(EPOSITION_POSITIONX, 0);
                drop(EPOSITION_POSITIONY, 0);
                drop(EPOSITION_WIDTH, header.width);
                drop(EPOSITION_HEIGHT, header.height);
            } else
                texture = EE_NULLTEXTURE;
        } else
            ekill("out of memory");
    } else
        ekill("out of memory");
    return result;
}

int ctarga::initialize (unsigned int width, unsigned int height, scolor color) {
    size_t total = (width*height);
    int result = EE_OK, index;
    if (total > 0) {
        if ((data = (unsigned char *) emalloc(total*EE_TARGA_PIXEL))) {
            for (index = 0; index < total; index++) {
                data[R(index*EE_TARGA_PIXEL)] = color.red;
                data[G(index*EE_TARGA_PIXEL)] = color.green;
                data[B(index*EE_TARGA_PIXEL)] = color.blue;
                data[A(index*EE_TARGA_PIXEL)] = color.alpha;
            }
            header.type[ETGATYPE_IMAGE] = 2;
            header.width = width;
            header.height = height;
            header.bpp = 32;
            if ((result = alpha()))
                if ((result = texturize())) {
                    crop(EPOSITION_POSITIONX, 0);
                    crop(EPOSITION_POSITIONY, 0);
                    crop(EPOSITION_WIDTH, header.width);
                    crop(EPOSITION_HEIGHT, header.height);
                    drop(EPOSITION_POSITIONX, 0);
                    drop(EPOSITION_POSITIONY, 0);
                    drop(EPOSITION_WIDTH, header.width);
                    drop(EPOSITION_HEIGHT, header.height);
                } else
                    texture = EE_NULLTEXTURE;
        } else
            ekill("out of memory");
    } else
        result = EE_ERROR;
    return result;
}

int ctarga::invisibility (scolor color, unsigned int threshold, bool revealing) {
    size_t total, index;
    int result = EE_OK;
    if ((data) && (mask)) {
        total = (header.height*header.width);
        for (index = 0; index < total; index++) {
            if ((data[R(index*EE_TARGA_PIXEL)] >= (unsigned char)EE_MAX(color.red-threshold, 0)) && 
                    (data[R(index*EE_TARGA_PIXEL)] <= (unsigned char)EE_MIN(color.red+threshold, 255)) &&
                        (data[G(index*EE_TARGA_PIXEL)] >= (unsigned char)EE_MAX(color.green-threshold, 0)) && 
                            (data[G(index*EE_TARGA_PIXEL)] <= (unsigned char)EE_MIN(color.green+threshold, 255)) &&
                                (data[B(index*EE_TARGA_PIXEL)] >= (unsigned char)EE_MAX(color.blue-threshold, 0)) && 
                                    (data[B(index*EE_TARGA_PIXEL)] <= (unsigned char)EE_MIN(color.blue+threshold, 255)) &&
                                        (((bool)mask[S(index*EE_TARGA_MASK)]) == revealing))
                data[A(index*EE_TARGA_PIXEL)] = EE_MAX(0, (color.alpha-((int)mask[T(index*EE_TARGA_MASK)])));
        }
        if (!(result = texturize()))
            texture = EE_NULLTEXTURE;
    } else
        result = EE_ERROR;
    return result;
}

int ctarga::invisibility (unsigned char value, unsigned int positionx, unsigned int positiony, unsigned int radius, bool revealing) {
    int cordx, cordy, result;
    unsigned int pixel;
    float distance;
    unsigned char treshold;
    if ((data) && (mask)) {
        for (cordx = 0; cordx < header.width; cordx++) {
            for (cordy = 0; cordy < header.height; cordy++) {
                distance = cmath::sqrt(cmath::pow2((cordx-((int)positionx)))+cmath::pow2((cordy-((int)positiony))));
                if (distance <= radius) {
                    treshold = (255-((distance/radius)*value));
                    pixel = ((header.width*((header.height-1)-cordy))+cordx);
                    if (((bool)mask[S(pixel*EE_TARGA_MASK)]) == revealing)
                        data[A(pixel*EE_TARGA_PIXEL)] = EE_MIN(EE_MAX(0, (data[A(pixel*EE_TARGA_PIXEL)]-treshold)), 255);
                }
            }
        }
        if (!(result = texturize()))
            texture = EE_NULLTEXTURE;
    } else
        result = EE_ERROR;
    return result;
}

int ctarga::alpha (unsigned int alpha) {
    unsigned char *converted;
    int bppmode, result = EE_OK;
    size_t total, index;
    if (data) {
        bppmode = (header.bpp/8);
        total = (header.height*header.width);
        if (bppmode <= 3) {
            if ((converted = (unsigned char *) emalloc(total*EE_TARGA_PIXEL))) {
                for (index = 0; index < total; index++) {
                    converted[R(index*EE_TARGA_PIXEL)] = data[R(index*bppmode)];
                    converted[G(index*EE_TARGA_PIXEL)] = data[G(index*bppmode)];
                    converted[B(index*EE_TARGA_PIXEL)] = data[B(index*bppmode)];
                    converted[A(index*EE_TARGA_PIXEL)] = alpha;
                }
                efree(data);
                data = converted;
                header.bpp = 32;
            } else
                ekill("out of memory");
        }
        if (result) {
            if ((mask = (unsigned char *) emalloc(total*EE_TARGA_MASK))) {
                for (index = 0; index < total; index++) {
                    if (!((bool)data[A(index*EE_TARGA_PIXEL)])) {
                        mask[S(index*EE_TARGA_MASK)] = false;
                        mask[T(index*EE_TARGA_MASK)] = 0;
                    } else {
                        mask[S(index*EE_TARGA_MASK)] = true;
                        mask[T(index*EE_TARGA_MASK)] = (255-data[A(index*EE_TARGA_PIXEL)]);
                    }
                }
            } else
                ekill("out of memory");
        }
    } else
        result = EE_ERROR;
    return result;
}

int ctarga::texturize (void) {
    int result = EE_OK;
    if (data) {
        if ((header.width < GL_MAX_TEXTURE_SIZE) && (header.height < GL_MAX_TEXTURE_SIZE)) {
            if (texture != EE_NULLTEXTURE) {
                glBindTexture(EE_EXTENSION, texture);
                glTexSubImage2D(EE_EXTENSION, 0, 0, 0, header.width, header.height, GL_RGBA, GL_UNSIGNED_BYTE, data);
            } else {
                glGenTextures(1, &texture);
                glBindTexture(EE_EXTENSION, texture);
                glTexParameteri(EE_EXTENSION, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(EE_EXTENSION, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(EE_EXTENSION, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(EE_EXTENSION, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexImage2D(EE_EXTENSION, 0, GL_RGBA8, header.width, header.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }
        } else
            ekill("texture out of size");
    } else
        result = EE_ERROR;
    return result;
}

scolor *ctarga::pixel (unsigned int positionx, unsigned int positiony) {
    scolor *pixel = NULL;
	unsigned int position = 0;
    if (data) {
        if ((positionx < header.width) && (positiony < header.height)) {
            position = ((header.width*((header.height-1)-positiony))+positionx)*EE_TARGA_PIXEL;
            if ((pixel = (scolor *) emalloc(sizeof(scolor)))) {
                pixel->red = data[R(position)];
                pixel->green = data[G(position)];
                pixel->blue = data[B(position)];
                pixel->alpha = data[A(position)];
            } else
                ekill("out of memory");
        }
    }
	return pixel;
}

GLuint ctarga::get (void) {
    GLuint result;
    if (returned)
        result = EE_NULLTEXTURE;
    else {
        result = texture;
        coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_POSITIONX] = coordinates[EPOSITIONSUBJECT_ELEMENT].destination[EPOSITION_POSITIONX];
        coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_POSITIONY] = coordinates[EPOSITIONSUBJECT_ELEMENT].destination[EPOSITION_POSITIONY];
        coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_WIDTH] = coordinates[EPOSITIONSUBJECT_ELEMENT].destination[EPOSITION_WIDTH];
        coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_HEIGHT] = coordinates[EPOSITIONSUBJECT_ELEMENT].destination[EPOSITION_HEIGHT];
        coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_POSITIONX] = coordinates[EPOSITIONSUBJECT_ELEMENT].source[EPOSITION_POSITIONX];
        coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_POSITIONY] = coordinates[EPOSITIONSUBJECT_ELEMENT].source[EPOSITION_POSITIONY];
        coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_WIDTH] = coordinates[EPOSITIONSUBJECT_ELEMENT].source[EPOSITION_WIDTH];
        coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_HEIGHT] = coordinates[EPOSITIONSUBJECT_ELEMENT].source[EPOSITION_HEIGHT];
        channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_R] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_R];
        channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_G] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_G];
        channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_B] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_B];
        channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_A] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_A];
    }
    returned = (!returned);
    return result;
}

void ctarga::restore (void) {
    crop(EPOSITION_POSITIONX, 0);
    crop(EPOSITION_POSITIONY, 0);
    crop(EPOSITION_WIDTH, header.width);
    crop(EPOSITION_HEIGHT, header.height);
    drop(EPOSITION_POSITIONX, 0);
    drop(EPOSITION_POSITIONY, 0);
    drop(EPOSITION_WIDTH, header.width);
    drop(EPOSITION_HEIGHT, header.height);
    channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_R] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_G] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_B] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_A] = 1.0f;
    channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_R] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_G] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_B] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_A] = 1.0f;
    rotation = 0.0f;
}

void ctarga::unload (void) {
    if (data)
        efree(data);
    if (mask)
        efree(mask);
    if (texture != EE_NULLTEXTURE)
        glDeleteTextures(1, &texture);
    this->init();
}
