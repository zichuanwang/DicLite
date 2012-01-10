#include "dict_input.h"

Dict_Input::Dict_Input() {
    for(int i = 0; i < 5; i++) {
        _para_status.para[i] = false;
        _parse_status = PARSE_SUCCESS;
    }
}

Dict_Input::~Dict_Input() {
    
}

void Dict_Input::resolve_para(int argc, char* argv[]) {
    char ch;
    while ((ch = getopt(argc, argv, "ehns")) != -1)
    {
        switch (ch) {
            case 'e':
                _para_status.para[eg] = true;
                break;
            case 'h':
                _para_status.para[help] = true;
                break;
            case 'n':
                _para_status.para[net] = true;
                break;
            case 's':
                _para_status.para[sugg] = true;
                break;
            case '?':
                _parse_status = PARSE_FAILURE;
                return;
        }
    }
    if(optind == argc) {
        if(argc > 1 && _para_status.para[help] == false)
            _parse_status = PARSE_FAILURE;
    }
    else {
        _word = argv[optind];
    }
    if(!this->has_error())
        resolve_status(argc);
}

void Dict_Input::resolve_status(int argc) {
    if(argc == 1) {
        _status_type = complicated;
    }
    else if(_para_status.para[help]){
        _status_type = help_page;
    }
    else {
        _status_type = normal;
    }
}