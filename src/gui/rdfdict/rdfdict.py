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

import sys, os
import rdflib

rdflib_version = [int(i) for i in rdflib.__version__.split('.')]
if rdflib_version <= [2,4,99]:
    from rdflib.RDFS import RDFSNS as RDFS
    XSD = rdflib.Namespace("http://www.w3.org/2001/XMLSchema#")
else:
    from rdflib.namespace import RDFS, XSD

class RDFdict(dict):
    def __init__(self, *args, **kwargs):
        self.update(dict(*args, **kwargs))
        self.graph = rdflib.ConjunctiveGraph()
        self.parsed_files = set()
    def parse(self, path, format="n3", subject=None):
        '''Parse a file into the RDFdict.graph even with no namespaces given. The RDF 
        schema "seeAlso" is supported and the files will be parsed recursively. '''
        if path in self.parsed_files:
            return
        self.parsed_files.add(path)
        graph = rdflib.ConjunctiveGraph()
        graph.parse(path, format=format)
        for s,p,o in graph.triples((subject, RDFS.seeAlso, None)):
            self.parse(o)
        self.graph += graph
    def structure(self, subject=None):
        '''Fills itself as a dictionary that represents the structure of the graph.
        Copies of dictionaries with nodes as subjects replace nodes that are objects. 
        ''' 
        self.update(self._structure(subject=subject))
    def _structure(self, subject=None):
        tree = RDFdict()
        tree.graph = self.graph
        for s,p,o in self.graph.triples((subject, None, None)):
                if s not in tree:
                    tree[s] = RDFdict() 
                if p not in tree[s]:
                    tree[s][p] = []
                if isinstance(o, rdflib.BNode):
                    tree[s][p].append(self._structure(subject=o))
                else:
                    tree[s][p].append(o)
        return tree
    def interpret(self, *args):
        '''Interprets itself according to the namespaces dictionaries that map strings 
        to RDFLib namespaces. The strings that form the keys of the namespaces replace the 
        RDFLib URIRefs. RDFLib literals are interpreted as ints, floats or unicode 
        according to their datatype even with no namespaces given.'''
        self.namespaces = {}
        for d in args:
            self.namespaces.update(d)
        interp_tree = self._interpret(self)
        self.clear()
        self.update(interp_tree)
    def _interpret(self, tree):
        if isinstance(tree, dict):
            interp_tree = RDFdict() 
            #interp_tree.graph = self.graph
            for key, item in tree.items():
                interp_key = self._interpret(key)
                if isinstance(item, dict):
                    interp_tree[interp_key] = self._interpret(item)
                elif isinstance(item, list):
                    interp_tree[interp_key] = [self._interpret(i) for i in item]
            return interp_tree
        else:
            return self._interpret_rdfobj(tree)
    def _interpret_rdfobj(self, obj):
            if isinstance(obj, rdflib.Literal):
                if(obj.datatype == XSD.integer):
                    return int(obj)
                elif((obj.datatype == XSD.float) or (obj.datatype == XSD.double) or (obj.datatype == XSD.decimal)):
                    return float(obj)
                else:
                    try:
                        return unicode(obj)
                    except NameError: #python3
                        return str(obj)
            elif isinstance(obj, rdflib.URIRef):
                for key, namespace in self.namespaces.items():
                    if obj.startswith(namespace):
                        return key + ":" + obj.split("#")[1]
                else:
                    return obj
            else:
                return obj
