#pragma once
#include <iostream>
#include <string.h>
using namespace std;

class Dict_Network{
public:
	Dict_Network();
	std::string get_word_from_net(std::string word);
private:
	std::string send_request(string &url);
};
