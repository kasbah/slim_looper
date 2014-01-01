import rdflib
import sys
import os
from pprint import pprint as pp

#path = "file://" + os.getcwd() + "/../lv2/slim.ttl"
path = "file:///home/kaspar/.lv2/slim_looper.lv2/manifest.ttl"
plugin = rdflib.Namespace("https://github.com/kasbah/slim_looper.lv2")

namespaces = {
        "xsd"     : rdflib.Namespace("http://www.w3.org/2001/XMLSchema#"),
        "rdfs"    : rdflib.Namespace("http://www.w3.org/2000/01/rdf-schema#"),
        "rdf"     : rdflib.Namespace("http://www.w3.org/1999/02/22-rdf-syntax-ns#"),
        "bufsize" : rdflib.Namespace("http://lv2plug.in/ns/ext/buf-size#"),
        "atom"    : rdflib.Namespace("http://lv2plug.in/ns/ext/atom#"),
        "lv2"     : rdflib.Namespace("http://lv2plug.in/ns/lv2core#"),
        "midi"    : rdflib.Namespace("http://lv2plug.in/ns/ext/midi#"),
        "owl"     : rdflib.Namespace("http://www.w3.org/2002/07/owl#"), 
        "doap"    : rdflib.Namespace("http://usefulinc.com/ns/doap#"),
        "slim"    : rdflib.Namespace("https://github.com/kasbah/slim_looper.lv2#"),
}


class Model(object):
    def __init__(self, subject=None, graph=None, format='n3'):
        if graph:
            self.graph = graph
        else:
            self.graph = rdflib.ConjunctiveGraph()
        self.subject = subject
        self.format = format
        self.parsed_files = set()
    def parse(self, path):
        if not path.startswith('file://'):
            path = os.path.realpath(path)
            assert os.path.exists(path)
            path = 'file://%s' % path

        if path in self.parsed_files:
            return
        self.parsed_files.add(path)
        graph = rdflib.ConjunctiveGraph()
        graph.parse(path, format=self.format)
        for extension in graph.triples([None, namespaces["rdfs"].seeAlso, None]):
            self.parse(extension[2])
        self.graph += graph


def interpret(obj):
    try:
        if isinstance(obj, rdflib.Literal):
            if(obj.datatype == namespaces["xsd"].integer):
                return int(obj.decode())
            elif(obj.datatype == namespaces["xsd"].float):
                return float(obj.decode())
            else:
                return unicode(obj.decode())
        elif isinstance(obj, rdflib.URIRef):
            for key, namespace in namespaces.items():
                if namespace in obj:
                    return key + ":" + obj.split("#")[1]
            else:
                return obj
        else:
            return obj
    except:
        return obj

def structure(graph, subject=None):
    tree = {}
    for s,p,o in graph.triples((subject, None, None)):
            s = interpret(s)
            p = interpret(p)
            if s not in tree:
                tree[s] = {} 
            if p not in tree[s]:
                tree[s][p] = []
            if isinstance(o, rdflib.BNode):
                tree[s][p].append(structure(graph, subject=o))
            else:
                tree[s][p].append(interpret(o))
    return tree

model = Model()
model.parse(path)
t = structure(model.graph, plugin)
pp(t)

                


