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

#ifndef __WEICWS_CORE_H__
#define __WEICWS_CORE_H__
#define PCRE_STATIC

#include <iostream>
#include <vector>
#include <string>

#include "weicws.h"

#include "pcrecpp.h"
#include "crfsuite.h"

#include "hash_map.hpp"

namespace weicws {

class WeiCWSEngine {
public:
    WeiCWSEngine();
    ~WeiCWSEngine();

    int load( const Option& opt );
    int segment( const std::string& sentence,
            std::vector<std::string>& result);

private:
    /*< define type of resource */
    typedef hash_map<std::string, int> map_t;
    typedef hash_map<std::string, std::string> map_ss_t;
    typedef hash_map<std::string, int>::const_iterator  map_ptr_t;
    typedef hash_set<std::string> set_t;
    /*< end of define */

    /*< define type of data */
    typedef std::string char_t;
    typedef std::string label_t;
    typedef std::string word_t;

    typedef struct {
        std::string raw;
        std::vector<char_t> forms;
        std::vector<label_t> labels;
        std::vector<label_t> extra_labels;
    } sentence_t;

    typedef std::string feature_t;
    typedef std::vector<feature_t> features_t;
    /*< end of define */

private:
    crfsuite_model_t * model;
    crfsuite_tagger_t * tagger;

private:
    set_t       lexicon;
    map_t       ngram_lexicon;
    map_t       lav_lexicon;
    map_t       rav_lexicon;
    map_ss_t    auto_lexicon;

    std::vector< std::vector<int> > lexicon_cache;

    int ngram_total;

    pcrecpp::RE url_pattern;
    pcrecpp::RE eng_pattern;

private:
    /*
     * Main process of pre-processing, includes converting
     * character from SBC to DBC, tagging sentence by rule
     *
     *  @param[in]      input       the input sentence
     *  @param[out]     output      the output sentence
     */
    void preprocess(const sentence_t &input,
            sentence_t &output);

    /*
     * Pre-tagging sentence with rule, result in
     * written in sentence.extra_labels
     *
     *  @param[in/out]  sentence    the sentence
     *  @param[in]      pattern     the regex pattern
     */
    void rule_tagging(sentence_t &sentence,
            const pcrecpp::RE &pattern);

    /*
     * Extract features from sentence.
     *
     *  @param[in]  sent    the sentence
     *  @param[in]  gid     the group id
     *  @return     crfsuite_instance_t
     */
    crfsuite_instance_t extract_sentence_features(
            const sentence_t &sent,
            bool append=false);

    /*
     * Extract features from character
     *
     *  @param[in]  sent        the sentence
     *  @param[in]  i           the index of the character
     *  @param[out] features    the features
     *  @return     int         number of features extracted
     */
    int extract_character_features(
            const sentence_t &sent,
            int i,
            std::vector<std::string> &features);

    /*
     * Extract basic features from character
     *
     *  @param[in]  sent        the sentence
     *  @param[in]  i           the index of the character
     *  @param[out] features    the features
     *  @return     int         number of features extracted
     */
    int extract_basic_character_features(
            const sentence_t &sent,
            int i,
            std::vector<std::string> &features);

    /*
     * Extract lexicon features from character
     *
     *  @param[in]  sent        the sentence
     *  @param[in]  i           the index of the character
     *  @param[out] features    the features
     *  @return     int         number of features extracted
     */
    int extract_lexicon_character_features(
            const sentence_t &sent,
            int i,
            std::vector<std::string> &features);

    /*
     * Extract auto parse result features from
     * character
     *
     *  !this function is not availiable till now
     *
     *  @param[in]  sent        the sentence
     *  @param[in]  i           the index of the character
     *  @param[out] features    the features
     *  @return     int         number of features extracted
     */
    int extract_auto_character_features(
            const sentence_t &sent,
            int i,
            std::vector<std::string> &features);

    /*
     * Extract mutual information features from 
     * character
     *
     *  @param[in]  sent        the sentence
     *  @param[in]  i           the index of the character
     *  @param[out] features    the features
     *  @return     int         number of features extracted
     */
    int extract_mi_character_features(
            const sentence_t &sent,
            int i,
            std::vector<std::string> &features);

    /*
     * Extract accessory varieties features from 
     * character
     *
     *  @param[in]  sent        the sentence
     *  @param[in]  i           the index of the character
     *  @param[out] features    the features
     *  @return     int         number of features extracted
     */
    int extract_av_character_features(
            const sentence_t &sent,
            int i,
            std::vector<std::string> &features);

    /*
     * Normalize a sentencen, convert SBC to DBC
     *
     *  @param[in]  input   the input sentence
     *  @param[out] output  the output sentence
     */
    void normalize(const sentence_t &input, sentence_t &output);

    /*
     *
     *
     *
     */
    int load_set(const char *filename, set_t &lexicon);

    /*
     *
     *
     *
     */
    int load_map(const char *filename, map_t &lexicon);

    /*
     *
     *
     *
     */
    int load_map_s(const char *filename, map_ss_t &lexicon);

    /*
     *
     *
     *
     */
    floatval_t MI(const std::string &x,
            const std::string &y);

    /*
     *
     *
     */
    std::string AVF(const std::string &bigram,
            const map_t &lexicon,
            const std::string prefix);
};

};

#endif  // end
