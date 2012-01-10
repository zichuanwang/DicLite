#include "dict_haici_xml_parser.h"

Dict_Haici_XML_Parser::Dict_Haici_XML_Parser() {}
Dict_Haici_XML_Parser::~Dict_Haici_XML_Parser() {}


#define COLOR_NONE      "\33[0m"
#define COLOR_BLACK     "\33[30m" 
#define COLOR_RED       "\33[31m"
#define COLOR_GREEN     "\33[32m" 
#define COLOR_YELLOW    "\33[33m" 
#define COLOR_BLUE      "\33[34m"
#define COLOR_MAGENTA   "\33[35m"
#define COLOR_CYAN      "\33[36m"
#define COLOR_WHITE     "\33[37m"

static void replace_all(std::string& str,const std::string& old_value,const std::string& new_value) {   
    while(true) {   
        std::string::size_type pos(0);   
        if((pos = str.find(old_value)) != std::string::npos)   
            str.replace(pos, old_value.length(), new_value);   
        else   
            break;   
    }    
}   

dict_pos_wmg_pair Dict_Haici_XML_Parser::get_def_item_sep(std::string &def_item) {
    size_t pos = def_item.find('.');
    dict_pos_wmg_pair pwp_ret;
    if(pos != std::string::npos) {
        std::string pos_temp = def_item.substr(0, pos + 1);
        std::string wmg_temp = def_item.substr(pos + 1);
        pwp_ret.pos = pos_temp;
        pwp_ret.wmg = wmg_temp;
        //printf("%s---%s",pos_temp.c_str(),wmg_temp.c_str());
    }
    else {
        pwp_ret.pos = "def.";
        pwp_ret.wmg = def_item;
    }
    return pwp_ret;
}

void Dict_Haici_XML_Parser::parse_def(std::string &def) {
    if(def == std::string("Not Found")) {
        //printf("not found\n");
        return;
    }
    //printf("def:%s\n",def.c_str());
    static char seps[] = "\t\n";
    char *token;
    dict_pos_wmg_pair pwp_temp;
    
    token = strtok(const_cast<char*>(def.c_str()), seps);
    while(token != NULL) {
        std::string token_str = token;
        pwp_temp = get_def_item_sep(token_str);
        this->pwp_vec.push_back(pwp_temp);
        token = strtok(NULL, seps);
    }
}

void Dict_Haici_XML_Parser::parse(CMarkup &xml) {
    if(!xml.FindElem("dict"))
        return;
    
    xml.IntoElem();
    if(xml.FindElem("key")) {
        this->key = xml.GetData();
    }
    if(xml.FindElem("lang")) {
        this->lang = xml.GetData();
    }
    if(xml.FindElem("audio")) {
        this->audio = xml.GetData();
    }
    if(xml.FindElem("pron")) {
        this->pron = "/";
        this->pron += xml.GetData();
        this->pron += "/";
    }
    if(xml.FindElem("def")) {
        if(this->lang == std::string("ec")) {
            std::string def = xml.GetData();
            this->parse_def(def);
        }
        else if(this->lang == std::string("ce")) {
            std::string def = xml.GetData();
            rel_vec.push_back(def);
        }
    }
    
    if(xml.FindElem("rel") && this->lang == std::string("ce")) {
        rel_vec.clear();
        rel_vec.push_back(xml.GetData());
        int i = 10;
        while(xml.FindElem("rel")) {
            if(i <= 1)
                break;
            rel_vec.push_back(xml.GetData());
            i--;
        }
    }
    while(xml.FindElem("sent")) {
        xml.IntoElem();
        dict_ost_tst_pair otp_temp;
        if(xml.FindElem("orig")) {
            otp_temp.ost = xml.GetData();
            replace_all(otp_temp.ost, "<em>", COLOR_RED);
            replace_all(otp_temp.ost, "</em>", COLOR_NONE);
        }
        if(xml.FindElem("trans")) {
            otp_temp.tst = xml.GetData();
        }
        this->otp_vec.push_back(otp_temp);
        xml.OutOfElem();
    }
    if(xml.FindElem("sugg")){
        sugg_vec.push_back(xml.GetData());
        int i = 5;
        while(xml.FindElem("sugg")) {
            if(i <= 1)
                break;
            sugg_vec.push_back(xml.GetData());
            i--;
        }
    }
}


void Dict_Haici_XML_Parser::parse(std::string &xml_str) {
    CMarkup xml;
    xml.SetDoc(xml_str);
    this->parse(xml);
}

void Dict_Haici_XML_Parser::parse(const char* file_name) {
    CMarkup xml;
    if(xml.Load(file_name) == false) {
        printf("load failed\n");
        return;
    }
    this->parse(xml);
}

void Dict_Haici_XML_Parser::display_std_info() {
    if(this->lang == std::string("ce")) {
        for(int i = 0; i < rel_vec.size(); i++) {
            printf("\33[33m%s\n\33[0m", rel_vec[i].c_str());
        }
    }
    else if(this->lang == std::string("ec")) {
        if(pron.length())
            printf("\33[35mpron.\33[0m\t\33[32m%s\33[0m\n", pron.c_str());
        for(int i = 0; i < pwp_vec.size(); i++) {
            printf("\33[35m%s\33[0m\t%s\n", pwp_vec[i].pos.c_str(), pwp_vec[i].wmg.c_str());
        }
    }
    else {
        for(int i = 0; i < sugg_vec.size(); i++) {
            if(i == 0)
                printf("\33[31mSuggestions:\33[0m\n");
            printf("%s\n", sugg_vec[i].c_str());
        }
    }
}

void Dict_Haici_XML_Parser::display_sentence() {
    printf("\n");
    for(int i = 0; i < otp_vec.size(); i++) {
        printf("\33[35m%d.\33[0m\t%s\n\t\33[36m%s\33[0m\n", i, otp_vec[i].ost.c_str(), otp_vec[i].tst.c_str());
    }
}

void Dict_Haici_XML_Parser::test_display() {
    printf("key:%s\n", key.c_str());
    if(this->lang == std::string("ce")) {
        for(int i = 0; i < rel_vec.size(); i++) {
            printf("rel:%s\n", rel_vec[i].c_str());
        }
    }
    else if(this->lang == std::string("ec")) {
        printf("pron:%s\n", pron.c_str());
        for(int i = 0; i < pwp_vec.size(); i++) {
            printf("pos:%s\nwmg:%s\n", pwp_vec[i].pos.c_str(), pwp_vec[i].wmg.c_str());
        }
        printf("audio:%s\n", audio.c_str());
    }
    else {
        for(int i = 0; i < sugg_vec.size(); i++) {
            printf("sugg:%s\n", sugg_vec[i].c_str());
        }
        return;
    }
    for(int i = 0; i < otp_vec.size(); i++) {
        printf("orig:%s\ntrans:%s\n", otp_vec[i].ost.c_str(), otp_vec[i].tst.c_str());
    }
}

std::string Dict_Haici_XML_Parser::get_vague_word(std::string xml_str) {
    this->parse(xml_str);
    std::string ret;
    ret.clear();
    if(sugg_vec.size() != 0) {
        ret = sugg_vec[0];
    }
    return ret;
}

void Dict_Haici_XML_Parser::display_key() {
    printf(("\33[33m%s\33[0m\n"), this->key.c_str());
}