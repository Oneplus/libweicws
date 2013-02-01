#include <iostream>
#include <vector>
#include <list>

#include "weicws.h"
#include "tinythread.h"
#include "fast_mutex.h"

using namespace std;
using namespace tthread;

const int MAX_LEN = 1024;

class Dispatcher {
public:
    Dispatcher( weicws::Model * model ) {
        _model = model;
    }

    int next(string &sentence) {
        sentence = "";
        lock_guard<fast_mutex> guard(_mutex);
        if (!getline(cin, sentence, '\n')) {
            return -1;
        }
        return 0;
    }

    void output(const vector<string> &result) {
        lock_guard<fast_mutex> guard(_mutex);
        for (int i = 0; i < result.size(); ++ i) {
            cout << result[i];
            cout << (i == result.size() - 1 ? '\n' : '|');
        }
        return;
    }

    weicws::Model * model() {
        return _model;
    }

private:
    fast_mutex      _mutex;
    weicws::Model * _model;
    string          _sentence;
};

void multithreaded_segment( void * args) {
    string sentence;
    vector<string> result;

    Dispatcher * dispatcher = (Dispatcher *)args;
    weicws::Model * model = dispatcher->model();

    while (true) {
        int ret = dispatcher->next(sentence);

        if (ret < 0)
            break;

        model->segment(sentence, result);
        dispatcher->output(result);
    }

    return;
}

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

    int num_threads = thread::hardware_concurrency();
    cerr << "TRACE: Running " << num_threads << " thread(s)" << endl;

    Dispatcher * dispatcher = new Dispatcher( &model );

    list<thread *> thread_list;
    for (int i = 0; i < num_threads; ++ i) {
        thread * t = new thread( multithreaded_segment, (void *)dispatcher );
        thread_list.push_back( t );
    }

    for (list<thread *>::iterator i = thread_list.begin();
            i != thread_list.end(); ++ i) {
        thread * t = *i;
        t->join();
        delete t;
    }

    return 0;
}

