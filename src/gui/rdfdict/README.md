#python-RDFdict
## About
Python-RDFdict is a small utility class that uses [RDFLib][1] to represent RDF as hierarchies. It is 
a subclass of `dict` with a few added methods that help parse and structure RDF. It turns 
itself into a dict of dicts and lists of dicts that represents a hierarchy according to nodes. 

It also allows for interpreting Literals as strings, ints and floats and URIRefs as strings according to the
namespaces you specify. Some namespaces are included in [namespaces.py](namespaces.py) for convenience.

I created this mainly to parse [lv2][2] .ttl files in python but it may be useful for other purposes.

[1]: http://librdf.org/
[2]: http://lv2plug.in/

## Usage
```python
from rdfdict import RDFdict
import namespaces as ns
import rdflib
from pprint import pprint

ttl_file = "http://lv2plug.in/export/899/trunk/plugins/eg01-amp.lv2/amp.ttl"
amp = rdflib.URIRef("http://lv2plug.in/plugins/eg-amp")

rdf_dict = RDFdict()

#we parse the file into the rdf_dict.graph which is a rdflib.ConjunctiveGraph
#the subject is optional but saves time parsing only what we care about
print("Fetching and parsing the ttl file.)
print("The fetching may take a while as it's an online resource.")
rdf_dict.parse(ttl_file, subject=amp)

#we populate rdf_dict with a structure
rdf_dict.structure()

#we replace the Literals with ints, floats and strings 
#and the URIRefs according to the namespaces we know about
rdf_dict.interpret(ns.lv2, ns.w3, ns.usefulinc, ns.kxstudio)

pprint(rdf_dict[amp]["lv2:port"])
```

should result in an output something like:

```
[{rdflib.term.BNode('ub2bL95C6'): {'lv2:index': [2],
                                   'lv2:name': ['Out'],
                                   'lv2:symbol': ['out'],
                                   'rdf:type': ['lv2:AudioPort',
                                                'lv2:OutputPort']}},
 {rdflib.term.BNode('ub2bL89C6'): {'lv2:index': [1],
                                   'lv2:name': ['In'],
                                   'lv2:symbol': ['in'],
                                   'rdf:type': ['lv2:AudioPort',
                                                'lv2:InputPort']}},
 {rdflib.term.BNode('ub2bL45C11'): {'lv2:default': [0.0],
                                    'lv2:index': [0],
                                    'lv2:maximum': [24.0],
                                    'lv2:minimum': [-90.0],
                                    'lv2:name': ['Gewinn',
                                                 'Увеличение',
                                                 'Gain',
                                                 'Gain',
                                                 '收益',
                                                 'Gain',
                                                 'Aumento',
                                                 '利益',
                                                 'Guadagno'],
                                    'lv2:scalePoint': [{rdflib.term.BNode('ub2bL85C7'): {'rdf:value': [-10.0],
                                                                                         'rdfs:label': ['-10']}},
                                                       {rdflib.term.BNode('ub2bL82C7'): {'rdf:value': [-5.0],
                                                                                         'rdfs:label': ['-5']}},
                                                       {rdflib.term.BNode('ub2bL76C18'): {'rdf:value': [5.0],
                                                                                          'rdfs:label': ['+5']}},
                                                       {rdflib.term.BNode('ub2bL79C7'): {'rdf:value': [0.0],
                                                                                         'rdfs:label': ['0']}}],
                                    'lv2:symbol': ['gain'],
                                    'rdf:type': ['lv2:InputPort',
                                                 'lv2:ControlPort']}}]
```
## License

    Copyright 2014 Kaspar Emanuel <kaspar.emanuel@gmail.com> 
    
    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.
    
    THIS SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.


