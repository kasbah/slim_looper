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

#include "slim_jack.h"
#include "slim_socket.h"
#include <jack/jack.h>

jack_port_t *input_port;
jack_port_t *output_port;

int slim_jack_process(jack_nframes_t n_samples, void* arg)
{
    return 0;
}

void slim_jack_shutdown (void* arg)
{
    exit(1);
}

int main(void)
{
    jack_client_t* client = jack_client_open("SLim Looper", 0 , 0);
    jack_set_process_callback(client, slim_jack_process, 0);
    jack_on_shutdown(client, slim_jack_shutdown, 0);
    input_port = jack_port_register (client, "in", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    output_port = jack_port_register (client, "out", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    jack_activate(client);
    slim_socket_run();
    exit(0);
}
