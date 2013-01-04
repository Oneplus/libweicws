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

