#pragma once
#include <iostream>
#include "Markup.h"
#include <string>
#include <vector>

typedef std::string part_of_speech;
typedef std::string word_meaning;
typedef std::string origin_sentene;
typedef std::string translated_sentence;

typedef struct {
    part_of_speech pos;
    word_meaning wmg;
} dict_pos_wmg_pair;

typedef struct {
    origin_sentene ost;
    translated_sentence tst;
} dict_ost_tst_pair;

class Dict_Haici_XML_Parser {
private:
    std::string key;
    std::string pron;
    std::string audio;
    std::string lang;
    std::vector<dict_pos_wmg_pair> pwp_vec;
    std::vector<dict_ost_tst_pair> otp_vec;
    std::vector<std::string> sugg_vec;
    std::vector<std::string> rel_vec;
public:
    Dict_Haici_XML_Parser();
    virtual ~Dict_Haici_XML_Parser();
    void parse(const char* file_name);
    void parse(std::string &xml_str);
    void display_std_info();
    void display_sentence();
    std::string get_vague_word(std::string xml_str);
    void display_key();
protected:
    void test_display();
    void parse(CMarkup &xml);
    void parse_def(std::string &def);
    static dict_pos_wmg_pair get_def_item_sep(std::string &def_item);
};