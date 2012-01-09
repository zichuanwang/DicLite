#pragma once

#include <iostream>
#include "dict_input.h"

class Dict_Client {
private:
    Dict_Input _config;
public:
    Dict_Client(); 
    ~Dict_Client();
    void init(Dict_Input &config) { _config = config; }
    void run();
protected:
    void search_word(std::string &word);
    void send_message_to_server(int code, std::string &msg);
    void receive_answer(char *buffer);
};