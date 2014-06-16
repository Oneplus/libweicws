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
#ifndef __PYWEICWS_H__
#define __PYWEICWS_H__

#include <string>
#include <vector>
#include "weicws.h"

class Segmentor {
public:
    Segmentor() {
    }

    ~Segmentor() {
    }

    void load(const std::string & model_path,
            const std::string & dict_path,
            const std::string & ngram_path,
            const std::string & lav_path,
            const std::string & rav_path,
            const std::string & auto_path) {
        option.set_model_path(model_path);
        option.set_dict_path(dict_path);
        option.set_ngram_path(ngram_path);
        option.set_lav_path(lav_path);
        option.set_rav_path(rav_path);
        option.set_auto_path(auto_path);

        model.load(option);
    }

    std::vector<std::string> segment(std::string sentence) {
        std::vector<std::string> ret;
        model.segment(sentence, ret);
        return ret;
    }
private:
    weicws::Option option;
    weicws::Model  model;
};

#endif  // end for __PYWEICWS_H__
