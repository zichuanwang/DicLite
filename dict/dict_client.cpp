#include <iostream>
#include <string>
#include "dict_client.h"
#include "dict_haici_xml_parser.h"
#include "dict_network.h"

#define BUFFER_SIZE 1024 * 8

Dict_Client::Dict_Client() {
}

Dict_Client::~Dict_Client() {
}

void Dict_Client::init(Dict_Input &config) { 
    _config = config; 
    if(_database.init() == false) {
        printf("Init database error");
        exit(1);
    }
}

void Dict_Client::stop() {
    _database.close();
}

static void display_prompt(std::string prom) {
    printf("\33[34m%s\33[0m", prom.c_str());
}

void Dict_Client::run() {
    if(_config.status() == help_page) {
        
    }
    else if(_config.status() == complicated) {
        std::string word;
        display_prompt("DicLite>");
        while (getline(std::cin, word))
        {
            if(word == std::string("quit")) {
                break;
            }
            search_word(word);
            display_prompt("DicLite>");
        }
    }
    else if(_config.status() == normal) {
        search_word(_config.word());
    }
    _database.close();
}

void Dict_Client::search_word(std::string &word) {
    std::string answer = search_word_help(word);
    if(_config.option().para[vague_search] == true) {
        Dict_Haici_XML_Parser paser;
        std::string vauge_word = paser.get_vague_word(answer);
        if(!vauge_word.empty())
            answer = this->search_word_help(vauge_word);
    }
    display_answer(answer);
}

std::string Dict_Client::search_word_help(std::string &word) {
    std::string answer;
    if(_config.option().para[network_only] == false) {
        answer = get_answer_in_database(word);
        if(!answer.empty())
            return answer;
    }
    answer = get_answer_through_network(word);
    if(!answer.empty())
        _database.save(word.c_str(), answer.c_str());
    return answer;
}

void Dict_Client::display_answer(std::string &answer) {
    Dict_Haici_XML_Parser parser;
    parser.parse(answer);
    if(_config.option().para[vague_search] == true)
        parser.display_key();
    if(_config.option().para[sentence]) {
        parser.display_sentence();
    }
    else 
        parser.display_std_info();
}

std::string Dict_Client::get_answer_in_database(std::string &word) {
    static char buffer[BUFFER_SIZE];
    std::string result;
    if(_database.search(word.c_str(), buffer) == SEARCH_SUCCESS) {
        result = buffer;
    }
    else {
        result.clear();
    }
    return result;
}

std::string Dict_Client::get_answer_through_network(std::string &word) {
    Dict_Network network;
    std::string answer = network.get_answer(word);
    return answer;
}
