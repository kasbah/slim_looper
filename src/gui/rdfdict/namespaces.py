#!/usr/bin/env python
# Copyright 2014 Kaspar Emanuel <kaspar.emanuel@gmail.com> 
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THIS SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

import rdflib
lv2 = {
    "lv2"             : rdflib.Namespace("http://lv2plug.in/ns/lv2core#"),
    "ui"              : rdflib.Namespace("http://lv2plug.in/ns/extensions/ui#"),
    "units"           : rdflib.Namespace("http://lv2plug.in/ns/extensions/units#"),
    "buf-size"        : rdflib.Namespace("http://lv2plug.in/ns/ext/buf-size#"),
    "midi"            : rdflib.Namespace("http://lv2plug.in/ns/ext/midi#"),
    "morph"           : rdflib.Namespace("http://lv2plug.in/ns/ext/morph#"),
    "options"         : rdflib.Namespace("http://lv2plug.in/ns/ext/options#"),
    "resizeports"     : rdflib.Namespace("http://lv2plug.in/ns/ext/resize-port#"),
    "urid"            : rdflib.Namespace("http://lv2plug.in/ns/ext/urid#"),
    "time"            : rdflib.Namespace("http://lv2plug.in/ns/ext/time#"),
    "log"             : rdflib.Namespace("http://lv2plug.in/ns/ext/log#"),
    "state"           : rdflib.Namespace("http://lv2plug.in/ns/ext/state#"),
    "port-props"      : rdflib.Namespace("http://lv2plug.in/ns/ext/port-props#"),
    "paramters"       : rdflib.Namespace("http://lv2plug.in/ns/ext/parameters#"),
    "data-access"     : rdflib.Namespace("http://lv2plug.in/ns/ext/data-access#"),
    "patch"           : rdflib.Namespace("http://lv2plug.in/ns/ext/patch#"),
    "dynamifest"      : rdflib.Namespace("http://lv2plug.in/ns/ext/dynmanifest#"),
    "atom"            : rdflib.Namespace("http://lv2plug.in/ns/ext/atom#"),
    "presets"         : rdflib.Namespace("http://lv2plug.in/ns/ext/presets#"),
    "instance-access" : rdflib.Namespace("http://lv2plug.in/ns/ext/instance-access#"),
    "uri-map"         : rdflib.Namespace("http://lv2plug.in/ns/ext/uri-map#"),
    "worker"          : rdflib.Namespace("http://lv2plug.in/ns/ext/worker#"),
    "port-groups"     : rdflib.Namespace("http://lv2plug.in/ns/ext/port-groups#"),
    "event"           : rdflib.Namespace("http://lv2plug.in/ns/ext/event#"),
    "meta"            : rdflib.Namespace("http://lv2plug.in/ns/meta#"),
}

kxstudio = { 
    "external-ui" : rdflib.Namespace("http://kxstudio.sourceforge.net/ns/lv2ext/external-ui#"),
    "programs"    : rdflib.Namespace("http://kxstudio.sourceforge.net/ns/lv2ext/programs#"),
    "rtmempool"   : rdflib.Namespace("http://kxstudio.sourceforge.net/ns/lv2ext/rtmempool"),
}

w3 = {
    "xsd"     : rdflib.Namespace("http://www.w3.org/2001/XMLSchema#"),
    "rdfs"    : rdflib.Namespace("http://www.w3.org/2000/01/rdf-schema#"),
    "rdf"     : rdflib.Namespace("http://www.w3.org/1999/02/22-rdf-syntax-ns#"),
    "owl"     : rdflib.Namespace("http://www.w3.org/2002/07/owl#"), 
}

usefulinc = {
    "doap"    : rdflib.Namespace("http://usefulinc.com/ns/doap#"),
}
