#!/usr/bin/env python

from __future__ import with_statement

import csegc
import os
import thread

from cStringIO import StringIO

"""
main class, wrapper to do some resource work
"""
class Segc(object):

    class MultipleInstance:
        pass
    
    lock = thread.allocate_lock()
    
    """
    overide __new__ to make it a singleton
    """
    def __new__(cls):
        with Segc.lock:
            if hasattr(cls, 'ins'):
                raise Segc.MultipleInstance
            cls.ins = object.__new__(cls)
        return cls.ins
    
    """
    load words from dictionary,
    if file not exist, raise an IOError
    otherwise try to insert the wordlist
    into dict
    """
    def load_dictionary(self, char_path, word_path):
        if(not os.path.exists(char_path) or not os.path.exists(word_path)):
            raise IOError
        with Segc.lock:
            csegc.acquire_global_resources(char_path, word_path)
            
    """
    create the algorithm object
    """
    def create_algorithm(self, text):
        al = Algorithm(text)
        return al

    def __del__(self):
        free_global_resources()

class Algorithm(object):

    def __init__(self, text):
        # hold it for reference counting
        self._text = text
        self._cobj = csegc.create_algor_object(self._text)

    def __del__(self):
        csegc.destroy_algor_object(self._cobj)
        
    def next_token_size(self):
        return csegc.get_algor_next_token(self._cobj)

    def tokenize(self):
        io = StringIO(self._text)
        t = []
        while(True):
            s = self.next_token_size()
            if s == 0:
                break
            t.append(io.read(s))
        return t

    def xtokenize(self):
        io = StringIO(self._text)
        while(True):
            s = self.next_token_size()
            if s == 0:
                return
            yield io.read(s)
    
    def __del__(self):
        csegc.destroy_algor_object(self._cobj)

