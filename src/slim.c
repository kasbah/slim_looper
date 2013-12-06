// 
// copyright 2013-2014 Kaspar Emanuel
// 
// This file is part of SLim Looper.
// 
// SLim Looper is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 3 as 
// published by the Free Software Foundation.
// 
// slim Looper is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with slim Looper. If not, see <http://www.gnu.org/licenses/>
// 

#include "slim.h"
#include <stdlib.h>
#include <string.h>

Slim* slim_new(uint32_t n_loopers, uint32_t max_n_samples)
{
    Slim* slim = (Slim*)malloc(sizeof(Slim));
    slim->n_loopers = n_loopers;
    slim->looper_array = calloc(n_loopers, sizeof(Looper));
    for (int i = 0; i < n_loopers; i++)
    {
        slim->looper_array[i] = looper_new(max_n_samples);
    }
    return slim;
}

void slim_activate(Slim* slim)
{
    for (int i = 0; i < (slim->n_loopers); i++)
    {
        looper_reset(slim->looper_array[i]);
        slim->looper_array[i]->input = slim->input;
    }
}

void slim_run(Slim* slim , uint32_t n_samples)
{
    memset(slim->output, 0, sizeof(float) * n_samples);
    for (int i = 0; i < (slim->n_loopers); i++)
    {
        looper_run(slim->looper_array[i], n_samples);
        for (int j = 0; j < n_samples; j++)
        {
            slim->output[j] += slim->looper_array[i]->output[j];
        }
    }
}

void slim_free(Slim* slim) 
{
    for (int i = 0; i < (slim->n_loopers); i++)
    {
        looper_free(slim->looper_array[i]);
    }
}
