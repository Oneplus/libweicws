#!/usr/bin/env python

unknownSample = "$NA$"

#####################################################################
#                                                                   #
#   Sentence class                                                  #
#                                                                   #
#####################################################################

class Sentence:
    default_encoding = "utf-8"

    def __init__(self, raw, train=False, encoding="utf-8"):
        self.raw = raw
        self.sentence = "".join(raw.split()).decode(encoding).encode(self.default_encoding)
        self.forms = [form.encode(self.default_encoding) for form in self.sentence.decode(encoding)]
        self.tags = [unknownSample] * len(self.forms)
        self.extra_tags = [unknownSample] * len(self.forms)
        self.train = train

        if train:
            self.goldwords = self.raw.split()
            self.goldtags = [unknownSample] * len(self.forms)

            i = 0
            for word in self.goldwords:
                L = len(word.decode(self.default_encoding))
                #print word, L
                if L == 1:
                    self.goldtags[i] = "S"
                else:
                    for j in range(L):
                        if j == 0:
                            self.goldtags[i + j] = "B"
                        elif j + 1 == L:
                            self.goldtags[i + j] = "E"
                        else:
                            self.goldtags[i + j] = "M"
                i += L

    def __str__(self):
        L = len(self.forms)
        if self.train:
            return "\t".join(["%s_%s_%s_%s" % (self.forms[i],
                self.goldtags[i],
                self.tags[i],
                self.extra_tags[i]) for i in range(L)])
        else:
            return "\t".join(["%s_%s_%s" % (self.forms[i],
                self.tags[i],
                self.extra_tags[i]) for i in range(L)])

    def __len__(self):
        return len(self.forms)

#####################################################################
#                                                                   #
#   Basic Segmemntor Class                                          #
#                                                                   #
#       + segment                                                   #
#       + valid                                                     #
#       + tag_as_word                                               #
#                                                                   #
#####################################################################

class Segmentor:
    def segment(self, sentence):
        # for overwrite
        pass

    def valid(self, sentence, start, end):
        ret = True

        for i in range(start, end):
            ret = ret and (sentence.tags[i] == unknownSample)
        return ret

    def tag_as_word(self, sentence, start, end, prefix, style):
        if style == 2:
            # tags in {"B", "I"} style
            for i in range(start, end):
                if i == start:
                    sentence.tags[i] = prefix + "B"
                else:
                    sentence.tags[i] = prefix + "I"

        elif style == 4:
            # tag in {"B", "M", "E", "S"} style
            if start + 1 == end:
                sentence.tags[start] = prefix + "S"
            else:
                for i in range(start, end):
                    if i == start:
                        sentence.tags[i] = prefix + "B"
                    elif i + 1 == end:
                        sentence.tags[i] = prefix + "E"
                    else:
                        sentence.tags[i] = prefix + "M"

        elif style == 6:
            # tag in {"B0", "B1", "B2", "M", "E", "S"} style
            pass


