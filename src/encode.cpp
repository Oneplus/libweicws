/*
 * Copyright (c) 2013, HIT-SCIR<ir.hit.edu.cn>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * Author: Yijia Liu<oneplus.lau@gmail.com>
 */

#include "core.h"

#include "ul_strlib_x.h"
#include <sstream>
#include <cmath>

#include "utf.h"

namespace weicws {

void WeiCWSEngine::rule_tagging(sentence_t &sentence,
        const pcrecpp::RE &pattern) {
    
    std::vector<std::string> buffer;
    std::string prefix = "", match = "";
    std::string::const_iterator start = sentence.raw.begin();
    std::string::const_iterator end = sentence.raw.end();
    pcrecpp::Arg *arg = NULL;
    pcrecpp::Arg arg0 = &match; arg = &arg0;
    
    int base = 0;
    int offset = 0;
    int consumed = -1;
    int num_chars = -1;

    num_chars = UTF::getCharactersFromUTF8String(
            sentence.raw,
            &buffer);

    while (start != end) {

        // no match exit
        if (!pattern.DoMatch(string(start, end),
                    pcrecpp::RE::UNANCHORED, 
                    &consumed, 
                    &arg, 
                    1)) {
            break;
        }

        prefix = std::string(start, start + consumed - match.size());

        num_chars = UTF::getCharactersFromUTF8String(
                prefix,
                &buffer);

        for (int i = 0; i < num_chars; ++ i) {
            if (buffer[i] != " " && 
                    buffer[i] != "\t" && 
                    buffer[i] != "\n" &&
                    buffer[i] != "\r") {
                ++ base;
            }
        }

        // match = std::string(what[0].first, what[0].second);

        num_chars = UTF::getCharactersFromUTF8String(
                match,
                &buffer);

        offset = 0;
        for (int i = 0; i < num_chars; ++ i) {
            if (buffer[i] != " " && 
                    buffer[i] != "\t" && 
                    buffer[i] != "\n" &&
                    buffer[i] != "\r") {
                ++ offset;
            }
        }

        bool flag = true;

        for (int i = base; i < base + offset; ++ i) {
            if (sentence.extra_labels[i] != "") {
                flag = false;
                break;
            }
        }

        if (flag) {
            for (int i = base; i < base + offset; ++ i) {
                if (offset == 1) {
                    sentence.extra_labels[i] = "S";
                } else if (i == base) {
                    sentence.extra_labels[i] = "B";
                } else if (i + 1 == base + offset) {
                    sentence.extra_labels[i] = "E";
                } else {
                    sentence.extra_labels[i] = "M";
                }
            }
        }

        base = base + offset;
        start = start + consumed;
    }

}

void WeiCWSEngine::preprocess(const sentence_t &input,
        sentence_t &output) {

    int len = input.forms.size();

    output.raw = input.raw;
    output.forms.resize(len);
    output.labels.resize(len);
    output.extra_labels.resize(len);

    // convert sbc2dbc
    for (int i = 0; i < len; ++ i) {
        output.forms[i] = SBC2DBC(input.forms[i]);
        // output.labels[i] = input.labels[i];
    }

    // detect 
    rule_tagging(output, url_pattern);

    // detect english word
    rule_tagging(output, eng_pattern);

    // cache lexicon
    std::vector<std::vector<std::string> >sub_sentence(len + 1, std::vector<std::string>(len + 1, ""));
    lexicon_cache.resize(len, std::vector<int>(3));
    for (int i = 0; i < len; ++ i) {
        lexicon_cache[i][0] = -1;
        lexicon_cache[i][1] = -1;
        lexicon_cache[i][2] = -1;
    }

    for (int i = 0; i < len; ++ i) {
        sub_sentence[i][i] = "";
        for (int j = i; j < len && j < i + 5; ++ j) {
            sub_sentence[i][j + 1] = sub_sentence[i][j] + output.forms[j];
            if (lexicon.find(sub_sentence[i][j + 1]) != lexicon.end()) {
                int l = j + 1 - i;

                if (l > lexicon_cache[i][0])
                    lexicon_cache[i][0] = l;

                if (l > lexicon_cache[j][1])
                    lexicon_cache[j][1] = l;

                for (int k = i + 1; k < j; ++ k)
                    if (l > lexicon_cache[k][2])
                        lexicon_cache[k][2] = l;
            }
        }

        int l = 0;
        int j = i;
        if (output.extra_labels[i] == "B") {
            while (j < len && output.extra_labels[j] != "E") {
                ++ l;
                ++ j;
            }
        } else if (output.extra_labels[i] == "S") {
            l = 1;
            ++ j;
        }

        if (j == len) {
            j = len - 1;
        }

        if (l > lexicon_cache[i][0]) 
            lexicon_cache[i][0] = l;
        if (l > lexicon_cache[j][1])
            lexicon_cache[j][1] = l;
        for (int k = i + 1; k < j; ++ k) {
            if (l > lexicon_cache[k][2]) {
                lexicon_cache[k][2] = l;
            }
        }
    }
}

crfsuite_instance_t WeiCWSEngine::extract_sentence_features(
        const sentence_t &raw_sentence,
        bool append) {

    sentence_t sentence;

    preprocess(raw_sentence, sentence);

    int len = sentence.forms.size();
    int L   = -1;
    crfsuite_instance_t inst;
    crfsuite_item_t item;
    crfsuite_attribute_t cont;

    crfsuite_dictionary_t *attrs = NULL;
    crfsuite_dictionary_t *labels = NULL;


    model->get_attrs(model, &attrs);
    model->get_labels(model, &labels);

    crfsuite_instance_init(&inst);
    inst.group = 0;

    L = labels->num(labels);

    for (int i = 0; i < len; ++ i) {
        std::vector<std::string> features;

        extract_character_features(
                sentence,
                i,
                features);

        int lid = -1, aid = -1;

        if (append) {
            lid = labels->get(labels, sentence.labels[i].c_str());
        } else {
            lid = labels->to_id(labels, sentence.labels[i].c_str());
            if (lid < 0) lid = L;
        }

        crfsuite_item_init(&item);

        /*fprintf(stderr, "%s\t", 
                sentence.labels[i].c_str());*/

        for (int j = 0; j < features.size(); ++j) {

            if (append) {
                aid = attrs->get(attrs, features[j].c_str());
            } else {
                aid = attrs->to_id(attrs, features[j].c_str());
            }

            /*fprintf(stderr, "%s(%d)%c",
                    features[j].c_str(),
                    aid,
                    (j + 1 < features.size() ? '\t' : '\n'));*/

            if (0 <= aid) {
                crfsuite_attribute_init(&cont);
                crfsuite_attribute_set(&cont, aid, 1.0);
                crfsuite_item_append_attribute(&item, &cont);
            }
        }
        crfsuite_instance_append(&inst, &item, lid);
        crfsuite_item_finish(&item);
    }

    // fprintf(stderr, "\n");
    return inst;
}

int WeiCWSEngine::extract_character_features(
        const sentence_t &sentence,
        int i,
        std::vector<std::string> &features) {

    extract_basic_character_features(
            sentence,
            i,
            features);

    extract_lexicon_character_features(
            sentence,
            i,
            features);

    extract_mi_character_features(
            sentence,
            i,
            features);

    extract_av_character_features(
            sentence,
            i,
            features);

    extract_auto_character_features(
            sentence,
            i,
            features);

    return 0;
}

int WeiCWSEngine::extract_basic_character_features(
        const sentence_t &sentence,
        int i,
        std::vector<std::string> &features) {
    int len = sentence.forms.size();

    std::string currch  = sentence.forms[i];
    std::string prevch  = (i - 1 >= 0) ? sentence.forms[i - 1]  : "__BOS__";
    std::string prev2ch = (i - 2 >= 0) ? sentence.forms[i - 2]  : "__BOS__";
    std::string nextch  = (i + 1 < len) ? sentence.forms[i + 1] : "__EOS__";
    std::string next2ch = (i + 2 < len) ? sentence.forms[i + 2] : "__EOS__";

    features.push_back("f11=" + prev2ch); // u-2
    features.push_back("f12=" + prevch);  // u-1
    features.push_back("f13=" + currch);  // u0
    features.push_back("f14=" + nextch);  // u+1
    features.push_back("f15=" + next2ch); // u+2

    features.push_back("f21=" + prev2ch + "/" + prevch);  // b-2,-1
    features.push_back("f22=" + prevch  + "/" + currch);  // b-1,0
    features.push_back("f23=" + currch  + "/" + nextch);  // b0,1
    features.push_back("f24=" + nextch  + "/" + next2ch); // b1,2

    features.push_back("f31=" + prev2ch + "/" + currch);
    features.push_back("f32=" + prevch  + "/" + nextch);
    features.push_back("f33=" + currch  + "/" + next2ch);

    features.push_back("f41=" + prevch  + "/" + currch + "/" + nextch);

    int tp = charType_ext(currch);

    features.push_back("f51=" + std::string(
            (tp == 0 ? "DIGIT" : (tp == 1 ? "LETTER" : (tp == 2 ? "PUNC" : "OTHER")))));

    if (currch == prev2ch) {
        features.push_back("f61");
    }
    if (prevch == nextch) {
        features.push_back("f62");
    }
    if (currch == next2ch) {
        features.push_back("f63");
    }
    if (currch == prevch) {
        features.push_back("f64");
    }
    if (currch == nextch) {
        features.push_back("f65");
    }

    return 0;
}

int WeiCWSEngine::extract_lexicon_character_features(
        const sentence_t &sentence,
        int i,
        std::vector<std::string> &features) {
    // lexicon features
    int ret = 0;
    if (lexicon_cache[i][0] > 0) {
        std::ostringstream S; S << lexicon_cache[i][0];
        features.push_back("f101=" + S.str());
        ++ ret;
    }

    if (lexicon_cache[i][1] > 0) {
        std::ostringstream S; S << lexicon_cache[i][1];
        features.push_back("f102=" + S.str());
        ++ ret;
    }

    if (lexicon_cache[i][2] > 0) {
        std::ostringstream S; S << lexicon_cache[i][2];
        features.push_back("f103=" + S.str());
        ++ ret;
    }

    return ret;
}

floatval_t WeiCWSEngine::MI(const std::string &x,
        const std::string &y) {

    std::string xy = x + "$$" + y;
    floatval_t pxy, px, py;

    if (ngram_lexicon.find(xy) != ngram_lexicon.end()) {
        pxy = (floatval_t)ngram_lexicon[xy] * ngram_total;
    } else {
        pxy = 0.1 * 0.1;
    }

    if (ngram_lexicon.find(x) != ngram_lexicon.end()) {
        px = (floatval_t)ngram_lexicon[x];
    } else {
        px  = 0.1;
    }

    if (ngram_lexicon.find(y) != ngram_lexicon.end()) {
        py = (floatval_t)ngram_lexicon[y];
    } else {
        py = 0.1;
    }

    return (log(pxy) - log(py) - log(px)) / log(2.0);
}

int WeiCWSEngine::extract_mi_character_features(
        const sentence_t &sentence,
        int i,
        std::vector<std::string> &features) {

    int len = sentence.forms.size();
    std::string currch  = sentence.forms[i];
    std::string prevch  = (i - 1 >= 0) ? sentence.forms[i - 1]  : "__BOS__";
    std::string nextch  = (i + 1 < len) ? sentence.forms[i + 1] : "__EOS__";

    floatval_t mi = 0.0;

    mi = (int)MI(prevch, currch);

    if (mi >= 15) {
        mi = 15;
    }
    if (mi <= -10) {
        mi = -10;
    }

    std::ostringstream S;
    S << mi;
    features.push_back("f201=" + S.str());

    mi = (int)MI(currch, nextch);

    if (mi >= 15) {
        mi = 15;
    }
    if (mi <= -10) {
        mi = -10;
    }

    S.clear(); S.str(""); S << mi;
    features.push_back("f202=" + S.str());

    return 0;
}

std::string WeiCWSEngine::AVF(const std::string &bigram,
        const map_t &lexicon,
        const std::string prefix) {

    int lvl = -1;
    map_ptr_t idx = lexicon.find(bigram);
    if (idx == lexicon.end()) {
        lvl = 0;
    } else if (idx->second >= 1 && idx->second <= 3) {
        lvl = 0;
    } else if (idx->second >= 4 && idx->second <= 8) {
        lvl = 1;
    } else if (idx->second >= 9 && idx->second <= 25) {
        lvl = 2;
    } else if (idx->second >= 26 && idx->second <= 100) {
        lvl = 3;
    } else {
        lvl = 4;
    }

    std::ostringstream S; S << lvl;
    return prefix + "=" + S.str();
}

int WeiCWSEngine::extract_av_character_features(
        const sentence_t &sentence,
        int i,
        std::vector<std::string> &features) {

    int L = sentence.forms.size();

    // quadgram
    std::string l41 = (sentence.forms[i] + 
            "$$" + (i + 1 < L ? sentence.forms[i + 1] : "__EOS__") +
            "$$" + (i + 2 < L ? sentence.forms[i + 2] : "__EOS__") +
            "$$" + (i + 3 < L ? sentence.forms[i + 3] : "__EOS__"));

    std::string l42 = ((i + 1 < L ? sentence.forms[i + 1] : "__EOS__") +
            "$$" + (i + 2 < L ? sentence.forms[i + 2] : "__EOS__") + 
            "$$" + (i + 3 < L ? sentence.forms[i + 3] : "__EOS__") + 
            "$$" + (i + 4 < L ? sentence.forms[i + 4] : "__EOS__"));

    std::string r41 = ((i - 3 >= 0 ? sentence.forms[i - 3] : "__BOS__") +
            "$$" + (i - 2 >= 0 ? sentence.forms[i - 2] : "__BOS__") + 
            "$$" + (i - 1 >= 0 ? sentence.forms[i - 1] : "__BOS__") +
            "$$" + sentence.forms[i]);

    std::string r42 = ((i - 4 >= 0 ? sentence.forms[i - 4] : "__BOS__") +
            "$$" + (i - 3 >= 0 ? sentence.forms[i - 3] : "__BOS__") +
            "$$" + (i - 2 >= 0 ? sentence.forms[i - 2] : "__BOS__") + 
            "$$" + (i - 1 >= 0 ? sentence.forms[i - 1] : "__BOS__"));

    // trigram
    std::string l31 = (sentence.forms[i] + 
            "$$" + (i + 1 < L ? sentence.forms[i + 1] : "__EOS__") +
            "$$" + (i + 2 < L ? sentence.forms[i + 2] : "__EOS__"));

    std::string l32 = ((i + 1 < L ? sentence.forms[i + 1] : "__EOS__") +
            "$$" + (i + 2 < L ? sentence.forms[i + 2] : "__EOS__") + 
            "$$" + (i + 3 < L ? sentence.forms[i + 3] : "__EOS__"));

    std::string r31 = ((i - 2 >= 0 ? sentence.forms[i - 2] : "__BOS__") + 
            "$$" + (i - 1 >= 0 ? sentence.forms[i - 1] : "__BOS__") +
            "$$" + sentence.forms[i]);

    std::string r32 = ((i - 3 >= 0 ? sentence.forms[i - 3] : "__BOS__") +
            "$$" + (i - 2 >= 0 ? sentence.forms[i - 2] : "__BOS__") + 
            "$$" + (i - 1 >= 0 ? sentence.forms[i - 1] : "__BOS__"));

    // bigram
    std::string l21 = (sentence.forms[i] + 
            "$$" + (i + 1 < L ? sentence.forms[i + 1] : "__EOS__"));

    std::string l22 = ((i + 1 < L ? sentence.forms[i + 1] : "__EOS__") +
            "$$" + (i + 2 < L ? sentence.forms[i + 2] : "__EOS__"));

    std::string r21 = ((i - 1 >= 0 ? sentence.forms[i - 1] : "__BOS__") +
            "$$" + sentence.forms[i]);

    std::string r22 = ((i - 2 >= 0 ? sentence.forms[i - 2] : "__BOS__") + 
            "$$" + (i - 1 >= 0 ? sentence.forms[i - 1] : "__BOS__"));

    features.push_back( AVF(l41, lav_lexicon, "f301") );
    features.push_back( AVF(l42, lav_lexicon, "f302") );
    features.push_back( AVF(r41, rav_lexicon, "f303") );
    features.push_back( AVF(r42, rav_lexicon, "f304") );

    features.push_back( AVF(l31, lav_lexicon, "f305") );
    features.push_back( AVF(l32, lav_lexicon, "f306") );
    features.push_back( AVF(r31, rav_lexicon, "f307") );
    features.push_back( AVF(r32, rav_lexicon, "f308") );

    features.push_back( AVF(l21, lav_lexicon, "f309") );
    features.push_back( AVF(l22, lav_lexicon, "f310") );
    features.push_back( AVF(r21, rav_lexicon, "f311") );
    features.push_back( AVF(r22, rav_lexicon, "f312") );

    return 0;
}

int WeiCWSEngine::extract_auto_character_features(
        const sentence_t &sentence,
        int i,
        std::vector<std::string> &features) {
    int L = sentence.forms.size();

    if (i + 1 == L)
        return 0;

    std::string gram = (sentence.forms[i] + "$$" + sentence.forms[i + 1]);

    if (auto_lexicon.find(gram) != auto_lexicon.end()) {
        features.push_back("f400=" + auto_lexicon[gram]);
    }

    return 0;
}

}
