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
#ifndef __WEICWS_H__
#define __WEICWS_H__

#include <string>
#include <vector>

namespace weicws {

class Option {

public:

    Option();
    ~Option();

public:
    void set_model_path(const std::string& model_path);
    void set_dict_path(const std::string& dict_path);
    void set_ngram_path(const std::string& ngram_path);
    void set_lav_path(const std::string& lav_path);
    void set_rav_path(const std::string& rav_path);
    void set_auto_path(const std::string& auto_path);

public:
    std::string model;
    std::string dict;
    std::string ngram;
    std::string lav;
    std::string rav;
    std::string autp;
};


class Model {
public:

    Model();
    ~Model();

public:
    /*
     * Load model and resource from option
     *
     *  @param[in]  opt the option
     */
    int load( const Option& opt );

    /*
     * Segment the input sentence
     *
     *  @param[in]  sentence    the sentence
     *  @param[out] result      the result
     */
    void segment( const std::string& sentence,
            std::vector<std::string>& result);

private:
    void * engine;
};

};

#endif  // end for __WEICWS_H__
