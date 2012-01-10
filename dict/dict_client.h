#pragma once

#include <iostream>
#include "dict_input.h"
#include "dict_database.h"

class Dict_Client {
private:
    Dict_Input _config;
    Dict_Database _database;
    bool _vague_search;
public:
    Dict_Client(); 
    ~Dict_Client();
    void init(Dict_Input &config);
    void run();
    void stop();
protected:
    void search_word(std::string &word);
    std::string search_word_help(std::string &word);
    void display_answer(std::string &answer);
    std::string get_answer_in_database(std::string &word);
    std::string get_answer_through_network(std::string &word);
    void sigroutine(int dunno);
    static void display_help_page();
};