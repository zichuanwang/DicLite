#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <string.h>
#include "dict_database.h"
#include "dict_network.h"

typedef enum {
    NORMAL,
    FUZZY,
} request_type;

typedef struct {
    request_type type;
    std::string word;
} request_struct;

class Dict_Server{
public:
    Dict_Server();
    ~Dict_Server();
    int run();
    
protected:
    bool init_database();
    const void init_daemon();
    Dict_Database database;
    Dict_Network network;
    std::string query_word(int code, const char* word);
};


#endif
