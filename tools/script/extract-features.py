#!/usr/bin/env python

import sys
import cPickle as pickle
from copy           import  deepcopy
from math           import  log
from ws             import  Sentence, unknownSample
from optparse       import  OptionParser
from chartypes_utf8 import  chartype, sbc2dbc
from rulews_engine  import  URLSegmentor, EnglishSegmentor,\
        SuspensionSegmentor

SP="$$"

#####################################################################
#                                                                   #
#   Initialize Options                                              #
#                                                                   #
#####################################################################

def InitializeOption(opt_parser):
    opt_parser.add_option("-d",
            "--dict",
            dest="dictfile",
            help="use to specify dict file path")

    opt_parser.add_option("-n",
            "--ngram",
            dest="ngram",
            help="use to specify ngram file path")

    opt_parser.add_option("-l",
            "--lav",
            dest="lav",
            help="use to specify left variety file path")

    opt_parser.add_option("-r",
            "--rav",
            dest="rav",
            help="use to specify right variety file path")

    opt_parser.add_option("-f",
            "--filepath",
            dest="filepath",
            help="use to specify file path")

#####################################################################
#                                                                   #
#   Extract Character-Based Feature                                 #
#                                                                   #
#####################################################################

def ExtractBasicCharacterFeatures(sentence, i):
    L = len(sentence)
    assert (i >= 0 and i < L)

    currch  = sentence.forms[i]
    prevch  = sentence.forms[i - 1] if i >= 1 else "__BOS__"
    prev2ch = sentence.forms[i - 2] if i >= 2 else "__BOS__"
    nextch  = sentence.forms[i + 1] if i + 1 < L else "__EOS__"
    next2ch = sentence.forms[i + 2] if i + 2 < L else "__EOS__"

    prevchty = chartype(prevch) if i >= 1 else "__BOT__"
    currchty = chartype(currch)
    nextchty = chartype(nextch) if i + 1 < L else "__BOT__"

    features = [
            "f11=" + prev2ch,
            "f12=" + prevch,
            "f13=" + currch,
            "f14=" + nextch,
            "f15=" + next2ch,

            "f21=" + prev2ch + "/" + prevch,
            "f22=" + prevch  + "/" + currch,
            "f23=" + currch  + "/" + nextch,
            "f24=" + nextch  + "/" + next2ch,

            "f31=" + prev2ch + "/" + currch,
            "f32=" + prevch  + "/" + nextch,
            "f33=" + currch  + "/" + next2ch,

            "f41=" + prevch  + "/" + currch + "/" + nextch,

            "f51=" + currchty,
            "f52=" + prevchty + "/" + currchty,
            "f53=" + currchty + "/" + nextchty,
            "f54=" + prevchty + "/" + currchty + "/" + nextchty]

    if prev2ch == currch:   features.append( "f61" )
    if prevch == nextch:    features.append( "f62" )
    if currch == next2ch:   features.append( "f63" )
    if prevch == currch:    features.append( "f64" )
    if currch == nextch:    features.append( "f65" )

    return features

#####################################################################
#                                                                   #
#   Cache Lexicon Features                                          #
#                                                                   #
#####################################################################

def CacheLexicon(sentence, dic):
    L = len(sentence)

    cache = [[-1, -1, -1] for i in xrange(L)]
    subsent = [["" for i in xrange(L + 1)] for j in xrange(L + 1)]

    for i in xrange(L):
        subsent[i][i] = ""
        for j in xrange(i, min(i + 5, L)):
            subsent[i][j + 1] = subsent[i][j] + sentence.forms[j]
            if subsent[i][j + 1] in dic:
                l = j + 1 - i
                if l > cache[i][0]: cache[i][0] = l
                if l > cache[j][1]: cache[j][1] = l
                for k in xrange(i + 1, j):
                    if l > cache[k][2]: cache[k][2] = l

    return cache

#####################################################################
#                                                                   #
#   Extract Sentence Feature                                        #
#                                                                   #
#####################################################################

def ExtractLexiconCharacterFeatures(sentence, i, cache):
    assert (i < len(cache))

    features = []
    if cache[i][0] > 0: features.append("f101=%d" % cache[i][0])
    if cache[i][1] > 0: features.append("f102=%d" % cache[i][1])
    if cache[i][2] > 0: features.append("f103=%d" % cache[i][2])

    return features

#####################################################################
#                                                                   #
#   Extract Character Feature                                       #
#                                                                   #
#####################################################################

def MI(x, y, ngram):
    total = float(ngram[0])
    ngramdic = ngram[1]

    xy = SP.join([x, y])
    #print xy

    try: pxy = ngramdic[xy] / total
    except: pxy = 0.1 * 0.1 / total / total

    try: px  = ngramdic[x] / total
    except: px  = 0.1 / total

    try: py  = ngramdic[y] / total
    except: py  = 0.1 / total

    return (log(pxy) - log(px) - log(py)) / log(2.0)

def ExtractMICharacterFeatures(sentence, i, ngram):
    L = len(sentence)
    assert i < L

    features = []

    #x = SP.join([sentence.forms[i - 2] if i >= 2 else "__BOS__",
    #    sentence.forms[i - 1] if i >= 1 else "__BOS__"])
    #y = SP.join([sentence.forms[i],
    #    sentence.forms[i + 1] if i + 1 < L else "__EOS__"])
    x = sentence.forms[i - 1] if i >= 1 else "__BOS__"
    y = sentence.forms[i]

    mi = int(MI(x, y, ngram))
    if mi >= 15: mi = 15
    if mi <= -10: mi = -10

    features.append( "f201=%d" % mi )

    #x = SP.join([sentence.forms[i - 1] if i >= 1 else "__BOS__",
    #    sentence.forms[i]])

    #y = SP.join([sentence.forms[i + 1] if i + 1 < L else "__EOS__",
    #    sentence.forms[i + 2] if i + 2 < L else "__EOS__"])

    x = sentence.forms[i]
    y = sentence.forms[i + 1] if i + 1 < L else "__EOS__"

    mi = int(MI(x, y, ngram))
    if mi >= 15: mi = 15
    if mi <= -10: mi = -10

    features.append( "f202=%d" % mi )

    return features

#####################################################################
#                                                                   #
#   Extract Character Feature                                       #
#                                                                   #
#####################################################################

def ExtractAVCharacterFeatures(sentence, i, lav, rav):
    L = len(sentence)
    features = []

    l41 = SP.join([sentence.forms[i],
        sentence.forms[i + 1] if i + 1 < L else "__EOS__",
        sentence.forms[i + 2] if i + 2 < L else "__EOS__",
        sentence.forms[i + 3] if i + 3 < L else "__EOS__"])
    l42 = SP.join([sentence.forms[i + 1] if i + 1 < L else "__EOS__",
        sentence.forms[i + 2] if i + 2 < L else "__EOS__",
        sentence.forms[i + 3] if i + 3 < L else "__EOS__",
        sentence.forms[i + 4] if i + 4 < L else "__EOS__"])
    r41 = SP.join([sentence.forms[i - 3] if i >= 3 else "__BOS__",
        sentence.forms[i - 2] if i >= 2 else "__BOS__",
        sentence.forms[i - 1] if i >= 1 else "__BOS__",
        sentence.forms[i]])
    r42 = SP.join([sentence.forms[i - 4] if i >= 4 else "__BOS__",
        sentence.forms[i - 3] if i >= 3 else "__BOS__",
        sentence.forms[i - 2] if i >= 2 else "__BOS__",
        sentence.forms[i - 1] if i >= 1 else "__BOS__"])

    l31 = SP.join([sentence.forms[i],
        sentence.forms[i + 1] if i + 1 < L else "__EOS__",
        sentence.forms[i + 2] if i + 2 < L else "__EOS__",])
    l32 = SP.join([sentence.forms[i + 1] if i + 1 < L else "__EOS__",
        sentence.forms[i + 2] if i + 2 < L else "__EOS__",
        sentence.forms[i + 3] if i + 3 < L else "__EOS__",])
    r31 = SP.join([sentence.forms[i - 2] if i >= 2 else "__BOS__",
        sentence.forms[i - 1] if i >= 1 else "__BOS__",
        sentence.forms[i]])
    r32 = SP.join([sentence.forms[i - 3] if i >= 3 else "__BOS__",
        sentence.forms[i - 2] if i >= 2 else "__BOS__",
        sentence.forms[i - 1] if i >= 1 else "__BOS__"])

    l21 = SP.join([sentence.forms[i],
        sentence.forms[i + 1] if i + 1 < L else "__EOS__",])
    l22 = SP.join([sentence.forms[i + 1] if i + 1 < L else "__EOS__",
        sentence.forms[i + 2] if i + 2 < L else "__EOS__",])
    r21 = SP.join([sentence.forms[i - 1] if i >= 1 else "__BOS__",
        sentence.forms[i]])
    r22 = SP.join([sentence.forms[i - 2] if i >= 2 else "__BOS__",
        sentence.forms[i - 1] if i >= 1 else "__BOS__"])

    def stravf(x, av, prefix):
        lvl = -1
        if x not in av:
            lvl = 0
        elif av[x] >= 1 and av[x] <= 3:
            lvl = 0
        elif av[x] >= 4 and av[x] <= 8:
            lvl = 1
        elif av[x] >= 9 and av[x] <= 25:
            lvl = 2
        elif av[x] >= 26 and av[x] <= 100:
            lvl = 3
        else:
            lvl = 4
        return "%s=%d" % (prefix, lvl)

    return [stravf(l41, lav, "f301"),
            stravf(l42, lav, "f302"),
            stravf(r41, rav, "f303"),
            stravf(r42, rav, "f304"),
            stravf(l31, lav, "f305"),
            stravf(l32, lav, "f306"),
            stravf(r31, rav, "f307"),
            stravf(r32, rav, "f308"),
            stravf(l21, lav, "f309"),
            stravf(l22, lav, "f310"),
            stravf(r21, rav, "f311"),
            stravf(r22, rav, "f312"),]

def ExtractRuleCharacterFeatures(sentence, i):

    features = []
    if sentence.tags[i] != unknownSample:
        features.append("f501=" + sentence.tags[i])

    if sentence.extra_tags[i] != unknownSample:
        features.append("f502=" + sentence.extra_tags[i])

    return features

#####################################################################
#                                                                   #
#   Extract Character Feature                                       #
#                                                                   #
#####################################################################

def ExtractCharacterFeatures(sentence, i, cache, ngram, lav, rav):
    return (ExtractBasicCharacterFeatures(sentence, i) + 
            ExtractRuleCharacterFeatures(sentence, i) +
            ExtractLexiconCharacterFeatures(sentence, i, cache) +
            ExtractMICharacterFeatures(sentence, i, ngram) +
            ExtractAVCharacterFeatures(sentence, i, lav, rav))


#####################################################################
#                                                                   #
#   Extract Sentence Feature                                        #
#                                                                   #
#####################################################################

def ExtractSentenceFeatures(sentence, dic, ngram, lav, rav):
    normalization = deepcopy(sentence)
    L = len(normalization)
    for i in xrange(L):
        normalization.forms[i] = sbc2dbc(normalization.forms[i]).encode("utf-8")

    cache = CacheLexicon(normalization, dic)
    features = [ExtractCharacterFeatures(normalization, i, cache, ngram, lav, rav)
            for i in xrange(L)]
    return features

#####################################################################
#                                                                   #
#   Main Process                                                    #
#                                                                   #
#####################################################################

if __name__=="__main__":
    opt_parser = OptionParser()

    InitializeOption(opt_parser)

    opt, args = opt_parser.parse_args()

    try:
        ngram = pickle.load(open(opt.ngram, "r"))
    except:
        print >> sys.stderr, "Failed to loadd ngram."
        exit(1)

    try:
        dic = pickle.load(open(opt.dictfile, "r"))
    except:
        print >> sys.stderr, "failed to load dict."
        exit(1)

    try:
        lav = pickle.load(open(opt.lav, "r"))
        rav = pickle.load(open(opt.rav, "r"))
    except:
        print >> sys.stderr, "failed to load varities"
        exit(1)

    try:
        fp=open(opt.filepath, "r")
    except:
        print >> sys.stderr, "failed to open file, direct to sys.stdin."
        fp = sys.stdin

    presegmentors = [URLSegmentor(style = 4),
            EnglishSegmentor(style = 4),
            SuspensionSegmentor(style = 4),]
            #SpaceSegmentor(),]

    for line in fp:
        sentence = Sentence(line.strip(), train=True)

        for segmentor in presegmentors:
            segmentor.segment(sentence)

        features = ExtractSentenceFeatures(sentence, dic, ngram, lav, rav)

        L = len(sentence)
        for i in xrange(L):
            print "%s\t%s" % (sentence.goldtags[i],
                    "\t".join(features[i]))
        print
