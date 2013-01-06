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
