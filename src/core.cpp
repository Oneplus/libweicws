/*
 * Libweicws - A micro blog oriented Chinese word segmentation system.
 * Copyright (c) 2013, HIT-SCIR<ir.hit.edu.cn>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/*
 * Author: Yijia Liu<oneplus.lau@gmail.com>
 */

#include "core.h"

#include "ul_strlib_x.h"
#include "utf.h"

#include <cstring>

namespace weicws {

WeiCWSEngine::WeiCWSEngine():url_pattern("((https?|ftp|file)"
    "://[-A-Za-z0-9+&@#/%?=~_|!:,.;]*[-A-Za-z0-9+&@#/%=~_|])"),
    eng_pattern("((\\w+)([\\-'\\.]\\w+)*)"){
}

WeiCWSEngine::~WeiCWSEngine() {
}

/* ==============================================
 * Utilities function group
 * ============================================== */
int WeiCWSEngine::load_set(const char *filename, set_t &lexicon) {
    char buff[2048];
    FILE *fp = fopen(filename, "r");

    if (NULL == fp) {
        return -1;
    }

    while (fgets(buff, 2048, fp)) {
        std::string line = chomp(std::string(buff));
        lexicon.insert(line);
    }

    fclose(fp);
    return 0;
}

int WeiCWSEngine::load_map(const char *filename, map_t &lexicon) {
    char buff[2048];
    FILE *fp = fopen(filename, "r");
    int ret;

    if (NULL == fp) {
        return -1;
    }

    while (fgets(buff, 2048, fp)) {
        std::string line = chomp(std::string(buff));
        int at = line.find_last_of('\t');

        if (at != std::string::npos) {
            ret = toInt(line.substr(at + 1));
            lexicon[line.substr(0, at)] = toInt(line.substr(at + 1));
        }
    }

    fclose(fp);
    return 0;
}

int WeiCWSEngine::load_map_s(const char *filename, map_ss_t &lexicon) {
    char buff[2048];
    FILE *fp = fopen(filename, "r");

    if (NULL == fp) {
    }

    while (fgets(buff, 2048, fp)) {
        std::string line = chomp(std::string(buff));
        int at = line.find_last_of('\t');

        if (at != std::string::npos) 
            lexicon[line.substr(0, at)] = line.substr(at + 1);
    }

    fclose(fp);
    return 0;
}


int WeiCWSEngine::load(const Option& opt) {

    int ret = -1;
    const char *lexicon_file = opt.dict.c_str();
    const char *ngram_file   = opt.ngram.c_str();
    const char *lav_file     = opt.lav.c_str();
    const char *rav_file     = opt.rav.c_str();
    const char *auto_file    = opt.autp.c_str();
    const char *model_file   = opt.model.c_str();

    /* ==========================================
     *  LOAD LEXICON
     * ========================================== */
    if (ret = load_set(lexicon_file, lexicon)) {
        // WARNING_LOG("Failed to load lexicon [%s]", lexicon_file);
        lexicon.clear();
    } else {
        // TRACE_LOG("%d words loaded.", lexicon.size());
    }

    /* ==========================================
     *  LOAD NGRAME
     * ========================================== */
    if (ret = load_map(ngram_file, ngram_lexicon)) {
        // WARNING_LOG("Failed to load ngram [%s]", ngram_file);
        ngram_lexicon.clear();
    } else {
        // TRACE_LOG("%d ngram is loaded.", ngram_lexicon.size());
    }

    ngram_total = ngram_lexicon["$$"];

    /*
    if (ret = load_map_s(auto_file, auto_lexicon)) {
        // WARNING_LOG("Failed to load auto parse [%s]", auto_file);
        auto_lexicon.clear();
    } else {
        // TRACE_LOG("%d auto lexicon is loaded.", auto_lexicon.size());
    }
    */


    if (ret = load_map(lav_file, lav_lexicon)) {
        // WARNING_LOG("Failed to load lav [%s]", lav_file);
        lav_lexicon.clear();
    } else {
        // TRACE_LOG("%d lav is loaded.", lav_lexicon.size());
    }

    if (ret = load_map(rav_file, rav_lexicon)) {
        // WARNING_LOG("Failed to load rav [%s]", rav_file);
        rav_lexicon.clear();
    } else {
        // TRACE_LOG("%d rav is loaded.", rav_lexicon.size());
    }

    if (model_file != NULL) {
        if (ret = crfsuite_create_instance_from_file(model_file, (void **)&model)) {
            return -1;
        }
    }

    if (ret = model->get_tagger(model, &tagger)) {
        return -1;
    }

    return 0;
}

int WeiCWSEngine::segment(const std::string& sentence, std::vector<std::string>& result) {

    result.clear();
    /* ==========================================
     *  SPLIT SENTENCE INTO CHARACTERS
     * ========================================== */

    sentence_t sent;
    sent.raw = chomp(sentence);

    if (sent.raw.size() == 0) {
        return 0;
    }

    std::vector<word_t> words = split(sent.raw);

    int len = words.size();

    for (int i = 0; i < len; ++ i) {
        std::vector<char_t> chars;
        int num_chars = UTF::getCharactersFromUTF8String(words[i], &chars);
        sent.labels.resize(num_chars, "");
        for (int j = 0; j < num_chars; ++ j) {
            char_t item = chars[j];
            sent.forms.push_back(item);
        }
    }

    sent.extra_labels.resize(sent.forms.size());

    /* ==========================================
     *  EXTRACT FEATURES
     * ========================================== */

    crfsuite_instance_t inst = extract_sentence_features(sent, false);

    /* ==========================================
     *  TAGGING THE SENTENCE
     * ========================================== */

    int ret = -1;
    int * output = NULL;
    floatval_t score = 0;
    std::string word = "";
    const char *label = NULL;

    len = inst.num_items;
    output = (int *)calloc(len, sizeof(int));

    if (!crfsuite_instance_empty(&inst)) {
        if (ret = tagger->set(tagger, &inst)) {
            // fail to set instance to tagger
            // free memory
            crfsuite_instance_finish(&inst);
            free(output);
            return -1;
        }

        if (ret = tagger->viterbi(tagger, output, &score)) {
            // fail to set instance to tagger
            // free memory
            crfsuite_instance_finish(&inst);
            free(output);
            return -1;
        }
    }

    /* =========================================
     *  CONVERT TO WORDS
     * ========================================= */

    crfsuite_dictionary_t *labels = NULL;
    if (ret = model->get_labels(model, &labels)) {
        return -1;
    }

    word = sent.forms[0];
    for (int i = 1; i < len; ++ i) {
        ret = labels->to_string(labels, output[i], &label);

        if ('S' == label[0] || 'B' == label[0]) {
            result.push_back(word);
            word = sent.forms[i];
        } else {
            word += sent.forms[i];
        }
    }

    result.push_back(word);

    // free memory
    crfsuite_instance_finish(&inst);
    free(output);

    return result.size();
}

}

