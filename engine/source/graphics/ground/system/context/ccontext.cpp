/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "ccontext.h"
inline eflipdirection& operator++ (eflipdirection& value) { return value = (value==EFLIPDIRECTION_NULL)?EFLIPDIRECTION_VERTICAL:eflipdirection(value+1); }
inline epositionsubject& operator++ (epositionsubject& value) { return value = (value==EPOSITIONSUBJECT_NULL)?EPOSITIONSUBJECT_FRAME:epositionsubject(value+1); }
inline eposition& operator++ (eposition& value) { return value = (value==EPOSITION_NULL)?EPOSITION_POSITIONX:eposition(value+1); }
