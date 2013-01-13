#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import re
from chartypes_utf8 import chartype
from ws import Sentence, Segmentor

def len_ext(str):
    ret = 0
    for ch in str:
        if not (ch == u' ' or ch == u'\t' or ch == u'\n' or ch == u'\r'):
            ret += 1
    return ret

#####################################################################
#                                                                   #
#   Segmentor in URL Rule                                           #
#                                                                   #
#####################################################################

class URLSegmentor(Segmentor):

    def __init__(self, prefix="URL+", style=4):
        patt="(https?|ftp|file)://[-A-Za-z0-9+&@#/%?=~_|!:,.;]*[-A-Za-z0-9+&@#/%=~_|]"
        self.regex   = re.compile(patt)
        self.prefix = prefix
        self.style  = style

    def segment(self, sentence):
        buff = sentence.raw

        base = 0
        while True:
            m = self.regex.search(buff)

            if m is not None:
                prefix = buff[:m.start(0)]
                base  += len_ext(prefix.decode(sentence.default_encoding))
                offset = len_ext(m.group(0).decode(sentence.default_encoding))

                if self.valid(sentence, base, base + offset):
                    self.tag_as_word(sentence,
                            base,
                            base + offset,
                            self.prefix,
                            self.style)

                base = base + offset
                buff = buff[m.end(0):]
            else:
                break

#####################################################################
#                                                                   #
#   Segment in English Rule                                         #
#                                                                   #
#####################################################################

class EnglishSegmentor(Segmentor):
    def __init__(self, prefix="EN+", style=4):
        self.english_regex = re.compile("(\w+)([\-'\.]\w+)*")
        self.prefix = prefix
        self.style = style

    def segment(self, sentence):
        buff = sentence.raw

        base = 0
        while True:
            m = self.english_regex.search(buff)

            if m is not None:
                prefix = buff[:m.start(0)]
                base += len_ext(prefix.decode(sentence.default_encoding))
                offset = len_ext(m.group(0).decode(sentence.default_encoding))

                if self.valid(sentence, base, base + offset):
                    self.tag_as_word(sentence,
                            base,
                            base + offset,
                            self.prefix,
                            self.style)

                base = base + offset
                buff = buff[m.end(0):]
            else:
                break

#####################################################################
#                                                                   #
#   Segment in Suspension Rule                                      #
#                                                                   #
#####################################################################

class SuspensionSegmentor(Segmentor):
    def __init__(self, prefix="SP+", style=4):
        self.regex = re.compile(ur"[\.|\u3002|\u2026]{2,140}")
        self.prefix = prefix
        self.style = style

    def segment(self, sentence):
        buff = sentence.raw.decode(sentence.default_encoding)

        base = 0
        while True:
            m = self.regex.search(buff)

            if m is not None:
                prefix = buff[:m.start(0)]
                base += len_ext(prefix)
                offset = len_ext(m.group(0))

                if self.valid(sentence, base, base + offset):
                    self.tag_as_word(sentence,
                            base,
                            base + offset,
                            self.prefix,
                            self.style)

                base = base + offset
                buff = buff[m.end(0):]
            else:
                break

#####################################################################
#                                                                   #
#   Punctuation in Suspension Rule                                  #
#                                                                   #
#####################################################################

class PuncSegmentor(Segmentor):
    def __init__(self, prefix="PN+", style=4):
        self.regex = re.compile(u"[^\u4e00-\u9fa5]")
        self.prefix = prefix
        self.style = style

    def segment(self, sentence):
        buff = sentence.raw.decode(sentence.default_encoding)

        base = 0
        while True:
            m = self.regex.search(buff)

            if m is not None:
                prefix = buff[:m.start(0)]
                base += len_ext(prefix)
                offset = len_ext(m.group(0))

                if self.valid(sentence, base, base + offset):
                    self.tag_as_word(sentence,
                            base,
                            base + offset,
                            self.prefix,
                            self.style)

                base = base + offset
                buff = buff[m.end(0):]
            else:
                break

#####################################################################
#                                                                   #
#   Number in Suspension Rule                                       #
#                                                                   #
#####################################################################

class NumberSegmentor(Segmentor):
    def __init__(self, prefix="NU+", style=4):
        self.regex = re.compile(u"(-?\d+)(\.\d+)?")
        self.prefix = prefix
        self.style = style

    def segment(self, sentence):
        buff = sentence.raw.decode(sentence.default_encoding)

        base = 0
        while True:
            m = self.regex.search(buff)

            if m is not None:
                prefix = buff[:m.start(0)]
                base += len_ext(prefix)
                offset = len_ext(m.group(0))
                #print "#", base, offset

                if self.valid(sentence, base, base + offset):
                    self.tag_as_word(sentence,
                            base,
                            base + offset,
                            self.prefix,
                            self.style)

                base = base + offset
                buff = buff[m.end(0):]
            else:
                break


class SmileySegmentor(Segmentor):
    def __init__(self, prefix="SM+", style=4):
        self.prefix = prefix
        self.style = style

        from smileybank import SMILEY_BANK
        self.bank = SMILEY_BANK

    def segment(self, sentence):
        pass


class SpaceSegmentor(Segmentor):
    def __init__(self, prefix="SC+", style=4):
        self.prefix = prefix
        self.style = style

    def segment(self, sentence):
        chars = sentence.raw.decode(sentence.default_encoding)
        L = len(chars)

        idx = 0
        for i in xrange(1, L):
            if chars[i] == " " and chars[i - 1] != " ":
                if sentence.extra_tags[idx] == self.prefix + "B":
                    sentence.extra_tags[idx] = self.prefix + "S"
                else:
                    sentence.extra_tags[idx] = self.prefix + "I"

            if i + 1 < L and chars[i] == " " and chars[i + 1] != " ":
                sentence.extra_tags[idx + 1] = self.prefix + "B"

            if chars[i] != " ":
                idx += 1

if __name__=="__main__":
    test_cases = [Sentence("please click here: http://t.cn/aurl"),
            Sentence("推荐一首歌《You got me》"),
            Sentence("推荐 一首 歌 《You got me》  ", train=True),
            Sentence("John-Smith凉宫"),
            Sentence("99.999"),
            Sentence("99.9999999 可行吗"),
            Sentence("试一试。"),
            Sentence("试一试。。。。。。"),
            Sentence("再试一。。试。。。。"),]

    segmentors = [URLSegmentor(),
            EnglishSegmentor(),
            SuspensionSegmentor(),
            SpaceSegmentor(),]

    for case in test_cases:
        for segmentor in segmentors:
            segmentor.segment(case)

    for case in test_cases:
        print case
        print "".join(["=" for i in xrange(30)])
