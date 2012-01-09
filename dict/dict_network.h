#pragma once
#include <iostream>
#include <string.h>
using namespace std;

class Dict_Network{
public:
	Dict_Network();
    ~Dict_Network();
    std::string get_answer(std::string word);
private:
	std::string send_request(string &url);
};
