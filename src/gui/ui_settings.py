import rdflib
from rdfdict.rdfdict import RDFdict 
import rdfdict.namespaces as ns

class slim_ui_settings(object):
    def __init__(self):
        path = "../lv2/slim.ttl"
        plugin = rdflib.URIRef("https://github.com/kasbah/slim_looper.lv2")
        ns_slim = {"slim" : rdflib.Namespace(plugin + "#")}
        
        rdf_dict = RDFdict()
        rdf_dict.parse(path, subject=plugin)
        rdf_dict.structure()
        rdf_dict.interpret(ns.lv2, ns.w3, ns.usefulinc, ns.kxstudio, ns_slim)
        
        self._global = []
        self.looper = []
        
        for bnodes in rdf_dict[plugin]["lv2:port"]:
            for bnode in bnodes:
                port = bnodes[bnode]
                if "slim:GlobalSetting" in port["rdf:type"]:
                    self._global.append(dict(port))
                if "slim:LooperSetting" in port["rdf:type"]:
                    self.looper.append(dict(port))

