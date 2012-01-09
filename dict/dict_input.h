#pragma once
#include <iostream>
#include <string>
#include <getopt.h>

#define PARSE_SUCCESS 0
#define PARSE_FAILURE 1

typedef enum {
    sentence,
    network_only,
    vague_search,
    audio,
    help,
} para_type;

typedef enum {
    complicated,
    normal,
    help_page,
} status_type;

typedef struct {
    bool para[5];
} para_status;

class Dict_Input {
private:
    para_status _para_status;
    status_type _status_type;
    std::string _word;
    bool _parse_status;
public:
    Dict_Input();
    ~Dict_Input();
    void resolve_para(int argc, char* argv[]);
    para_status option() { return _para_status; }
    std::string& word(){ return _word; }
    bool has_error() {return _parse_status == PARSE_FAILURE; }
    status_type status(){ return _status_type; }
    void display_usage() {
        printf("usage: dict [-snvah] [word]\n");
    }
protected:
    void resolve_status(int argc);
};