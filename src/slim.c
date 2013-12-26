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
#include <stdio.h>

Slim* slim_new(uint32_t n_loopers, uint32_t max_n_samples)
{
    Slim* slim = (Slim*)malloc(sizeof(Slim));
    slim->n_loopers = n_loopers;
    slim->looper_array = calloc(n_loopers, sizeof(Looper));
    slim->socket = slim_socket_server_new();
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
    }
}

static void slim_parse_looper_message(Slim* slim, const SlimMessage msg)
{
    if ((msg.looper.number) >= 0 && (msg.looper.number <= (slim->n_loopers)))
    {
        LooperSettings* settings = slim->looper_array[msg.looper.number]->settings; 
        switch (msg.looper.command)
        {
            case SlimMessage_Looper_Command_RECORD:
                if (settings->state != RECORDING)
                {
                    settings->state = RECORDING;
                    settings->record_mode = MODE_NEW;
                }
                else
                {
                    settings->state = PLAYING;
                }
                break;
            case SlimMessage_Looper_Command_PAUSE:
                if (settings->state != PAUSED)
                {
                    settings->state = PAUSED;
                }
                else
                {
                    settings->state = PLAYING;
                }
                break;
        }
    }
}

static void slim_parse_global_message(Slim* slim, const SlimMessage msg)
{
}


static void slim_parse_messages(Slim* slim, const uint32_t n_bytes, const char* const msg_buffer)
{
    pb_istream_t stream = pb_istream_from_buffer(msg_buffer, n_bytes);

    int status = 1;
    int i = 0;
    while (status == 1)
    {
        if (i++ >= MAX_MESSAGES)
            break;
        SlimMessage* messages = slim->messages;
        status = pb_decode_delimited(&stream, SlimMessage_fields, &messages[i]);
        if (status == 1)
        {
            //switch(messages[i].type)
            //{
            //    case SlimMessage_Type_LOOPER:
            //        slim_parse_looper_message(slim, messages[i]);
            //        break;
            //    case SlimMessage_Type_GLOBAL:
            //        slim_parse_global_message(slim, messages[i]);
            //        break;
            //}

            printf ("command: %i\r\n", messages[i].looper.command);
            printf ("looper number: %i\r\n", messages[i].looper.number);
            if (messages[i].looper.command == SlimMessage_Looper_Command_SET)
            {
                for (int i = 0; i < messages[i].looper.settings_count; i++)
                {
                    SlimMessage_Looper_Setting setting = messages[i].looper.settings[i];
                    printf("setting: %i %f\r\n", setting.name , setting.value);
                }
            }
        }
    }
}


void slim_run(Slim* slim , uint32_t n_samples)
{
    char* msg_buffer = slim->msg_buffer;
    int n = slim_socket_server_read(slim->socket, msg_buffer);
    if (n > 0)
    {
        printf("n: %i\r\n", n);
        slim_parse_messages(slim, n, msg_buffer);
    }

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
    slim_socket_free(slim->socket);
    for (int i = 0; i < (slim->n_loopers); i++)
    {
        looper_free(slim->looper_array[i]);
    }
}

void slim_connect(Slim* slim, void* input, void* output) 
{
    for (int i = 0; i < (slim->n_loopers); i++)
    {
        for (int i = 0; i < (slim->n_loopers); i++)
        {
            slim->looper_array[i]->input = (const float*)input;
        }
    }
    slim->output = (const float*) output;
}


//void slim_work_loop(Slim* slim)
//{
//    while (1)
//    {
//        int n = read(socket->connection_fd, buffer, 255);
//        if (n < 0) perror("ERROR reading from socket");
//        else if (n > 0)
//        {
//            msg = slim_message__unpack(NULL, n, buffer);
//            if (msg > 0)
//            {
//                switch(msg->type)
//                {
//                    case SLIM_MESSAGE__TYPE__LOOPER:
//                        printf ("command: %i\r\n", msg->looper->command);
//                        if (msg->looper->command == SLIM_MESSAGE__LOOPER__COMMAND__SET)
//                        {
//                            for (int i = 0; i < (msg->looper->n_settings); i++)
//                            {
//                                SlimMessage__Looper__Setting* setting = msg->looper->settings[i];
//                                char* name = slim_message__looper__setting__name__descriptor.values[setting->name].name;
//                                printf("setting: %i %s %f\r\n", setting->name, name, setting->value);
//                            }
//                        }
//                        break;
//                }
//            }
//            else if (msg == NULL)
//            {
//                perror("ERROR unpacking message");
//            }
//        }
//    }
//    slim_socket_close(socket);
//}
