#include "looper.h"

// is position after processing nsamples before loop end?
static uint8_t slim_loop_pos_before_end(Loop* loop, uint32_t n_samples)
{
    return ((loop->pos + n_samples) <= (loop->end));
}

// existence is defined as being at least one block long
static uint8_t slim_loop_exists(Loop* loop, uint32_t n_samples)
{
    return (loop->end >= n_samples);
}


void
slim_record(Looper* looper, uint32_t n_samples)
{
    const float* const input  = looper->input;
    float* const       output = looper->output;

    Loop*               loop     = looper->loop;
    LooperSettings*     settings = looper->settings;

    switch(settings->record_mode) 
    {
        case MODE_NEW:
            //reset if we are not already running
            if (looper->previous_state != RECORDING)
            {
                loop->pos = 0;
                loop->end = 0;
            }
            memcpy(&(loop->buffer[loop->pos]), input, n_samples * sizeof(float));
            loop->pos += n_samples;
            loop->end += n_samples;
            memset(output, 0, n_samples * sizeof(float));
            break;

        case MODE_OVERDUB:
            if (slim_loop_pos_before_end(loop, n_samples)) 
            {
                memcpy(output, &(loop->buffer[loop->pos]), n_samples * sizeof(float));
                for (int i = 0; i < n_samples; i++)
                {   //TODO: reduce gain to stop clipping 
                    loop->buffer[loop->pos + i] += input[i];
                }
                loop->pos += n_samples;
            }
            //position is greater than loop length 
            //looping around to start as long as we have a loop
            else if (slim_loop_exists(loop, n_samples)) 
            {
                memcpy(output, loop->buffer, n_samples * sizeof(float));
                for (int i = 0; i < n_samples; i++)
                {//TODO: reduce gain to stop clipping 
                    loop->buffer[i] += input[i];
                }
                loop->pos = n_samples;
            }
            else //no loop, output silence
            {
                memset(output, 0, n_samples * sizeof(float));
            }
            break;

        case MODE_INSERT:
            if (slim_loop_exists(loop, n_samples))
            {
                memmove(&(loop->buffer[loop->pos + n_samples]), &(loop->buffer[loop->pos]), (loop->end - loop->pos) * sizeof(float));
                memcpy(&(loop->buffer[loop->pos]), input, n_samples * sizeof(float));
                loop->pos += n_samples;
                loop->end += n_samples;
            }
            memset(output, 0, n_samples * sizeof(float));
            break;

        case MODE_REPLACE:
            if (slim_loop_pos_before_end(loop, n_samples)) 
            {
                memcpy(&(loop->buffer[loop->pos]), input, n_samples * sizeof(float));
                loop->pos += n_samples;
            }
            //position is greater than loop length 
            //looping around to start as long as we have a loop
            else if (slim_loop_exists(loop, n_samples))
            {
                memcpy(loop->buffer, input, n_samples * sizeof(float));
                loop->pos = n_samples;
            }
            memset(output, 0, n_samples * sizeof(float));
            break;

        default:
            break;
    }
}

void
slim_play(Looper* looper, uint32_t n_samples)
{
    const float* const input  = looper->input;
    float* const       output = looper->output;

    Loop*               loop     = looper->loop;
    LooperSettings*     settings = looper->settings;

    if (slim_loop_pos_before_end(loop, n_samples))
    {
        memcpy(output, &(loop->buffer[loop->pos]), n_samples * sizeof(float));
        loop->pos += n_samples;
    }
    //position is greater than loop length 
    //looping around to start as long as we have a loop
    else if (slim_loop_exists(loop, n_samples))
    {
        memcpy(output, loop->buffer, n_samples * sizeof(float));
        loop->pos = n_samples;
    }
    else //no loop, output silence
    {
        memset(output, 0, n_samples * sizeof(float));
    }
}
