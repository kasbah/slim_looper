// 
// copyright 2013-2014 Kaspar Emanuel
// 
// This file is part of SLim Looper.
// 
// SLim Looper is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 3 as 
// published by the Free Software Foundation.
// 
// SLim Looper is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with SLim Looper. If not, see <http://www.gnu.org/licenses/>
// 

#ifndef __SLIM_H__
#define __SLIM_H__

#include "looper.h"

typedef struct {
    const float* input;
    float* output;
    uint32_t n_loopers;
    Looper** looper_array;
} Slim;

Slim* slim_new(uint32_t n_loopers);
void slim_activate(Slim* slim);
void slim_run(Slim* slim , uint32_t n_samples);
void slim_free(Slim* slim);
#endif// __SLIM_H__
