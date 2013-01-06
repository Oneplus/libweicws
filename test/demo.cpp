#include <iostream>
#include <vector>

#include "weicws.h"

using namespace std;

const int MAX_LEN = 1024;

int main() {
    string sentence;
    vector<string> result;

    weicws::Model model;
    weicws::Option option;

    option.set_model_path( "./data/model" );
    option.set_dict_path( "./data/dict" );
    option.set_ngram_path( "./data/ngram" );
    option.set_lav_path( "./data/lav" );
    option.set_rav_path( "./data/rav" );

    if (model.load( option ) < 0) {
        cerr << "ERROR: Failed to load model" << endl;
        return -1;
    }

    cerr << "TRACE: Loading model is done" << endl;

    while (getline(cin, sentence, '\n')) {
        model.segment(sentence, result);

        for (int i = 0; i < result.size(); ++ i) {
            cout << result[i];
            cout << (i == result.size() - 1 ? '\n' : '|');
        }
    }
    return 0;
}

