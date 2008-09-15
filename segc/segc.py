import csegc
import os

"""
main class, wrapper to do some resource work
"""
class Segc(object):

    """
    overide __new__ to make it a singleton
    """
    def __new__(cls):
        if not hasattr(cls, 'ins'):
            cls.ins = object.__new__(cls)
            cls._needinit = True
            cls._needdel = True
        return cls.ins
    
    def __init__(self):
        if Segc._needinit:
            csegc.pool_init()
            csegc.dict_init()
            Segc._needinit = False

    """
    load words from dictionary,
    if file not exist, raise an IOError
    otherwise try to insert the wordlist
    into dict
    """
    def load_words(self, fn):
        if(not os.path.exists(fn)):
            raise IOError
        csegc.dict_load_words(fn)

    """
    load chars from dictionary,
    if file not exist, raise an IOError
    otherwise try to insert the wordlist
    into dict
    """    
    def load_chars(self, fn):
        if(not os.path.exists(fn)):
            raise IOError
        csegc.dict_load_chars(fn)

    """
    create the algorithm object
    """
    def create_algorithm(self):
        al = Algorithm(self)
        return al

    def __del__(self):
        if Segc._needdel:
            csegc.dict_deinit()
            csegc.pool_deinit()
            Segc._needdel = False

class Algorithm(object):
    """
    overide __new__ to make it singleton
    """
    def __new__(cls, segc):
        if not isinstance(segc, Segc) or segc._needinit:
            raise TypeError

        if not hasattr(cls, 'ins'):
            cls.ins = object.__new__(cls)
            cls._needinit = True
            cls._needdel = True
        return cls.ins

    def __init__(self, segc):
        if Algorithm._needinit:
            csegc.algo_init()
            Algorithm._needinit = False

    def next_token_size(self, text):
        return csegc.algo_next_token(text)

    def next_token_text(self, text):
        s = cseg.algo_next_token(text)
        return text[:s]

    def tokenize(self, text):
        t = []
        while(True):
            s = self.next_token_size(text)
            if s == 0:
                break
            t.append(text[:s])
            text = text[s:]
        return t

    def xtokenize(self, text):
        while(True):
            s = self.next_token_size(text)
            if s == 0:
                return
            yield text[:s]
            text = text[s:]
    
    def __del__(self):
        if Algorithrm._needdel:
            csegc.algo_deint()
            Algorithm._needdel = False
