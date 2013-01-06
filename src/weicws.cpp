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
#include "weicws.h"
#include "core.h"

namespace weicws {

Option::Option() {
    model   = "";
    dict    = "";
    ngram   = "";
    lav     = "";
    rav     = "";
    autp    = "";
}

Option::~Option() {
}

void Option::set_model_path(const std::string& model_path) {
    model = model_path;
}

void Option::set_dict_path(const std::string& dict_path) {
    dict = dict_path;
}

void Option::set_ngram_path(const std::string& ngram_path) {
    ngram = ngram_path;
}

void Option::set_lav_path(const std::string& lav_path) {
    lav = lav_path;
}

void Option::set_rav_path(const std::string& rav_path) {
    rav = rav_path;
}

void Option::set_auto_path(const std::string& auto_path) {
    autp = auto_path;
}


Model::Model() {
    engine = NULL;
    WeiCWSEngine * cws_engine = new WeiCWSEngine();
    engine = reinterpret_cast< void * >(cws_engine);
}

Model::~Model() {
}

int Model::load( const Option& opt ) {
    return reinterpret_cast< WeiCWSEngine * >(engine)->load( opt );
}

void Model::segment( const std::string& sentence,
        std::vector<std::string>& result) {
    reinterpret_cast< WeiCWSEngine * >(engine)->segment( sentence, result );
}

}

