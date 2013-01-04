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
