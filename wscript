#!/usr/bin/env python

# Mandatory variables
top = '.'
out = 'build'

def options(opt):
    opt.recurse("src")

def configure(conf):
    conf.recurse("src")

def build(bld):
    bld.recurse("src")

def test(opt):
    opt.recurse("test")
