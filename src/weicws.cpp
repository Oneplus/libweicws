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

