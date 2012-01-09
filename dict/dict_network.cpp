#include "dict_network.h"
#include <iostream>
#include "curl/curl.h"
#include <ctime>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <string.h>
#include "dict_database.h"

#define kICiBaQueryApi "http://dict-co.iciba.com/api/dictionary.php?w="
#define kHaiciQueryApi "http://api.dict.cn/ws.php?utf8=true&q="

using namespace std;
static char errorBuffer[CURL_ERROR_SIZE];  

// This is the writer call back function used by curl  
static size_t writer(char *data, size_t size, size_t nmemb, std::string *buffer) {  
		// What we will return  
		size_t result = 0;  
		// Is there anything in the buffer?  
		if (buffer != NULL) {  
				// Append the data to the buffer  
				buffer->append(data, size * nmemb);  
				// How much did we write?  
				result = size * nmemb;  
		}  
		return result;  
}  

Dict_Network::Dict_Network()
{
}

Dict_Network::~Dict_Network()
{
}

std::string Dict_Network::get_answer(std::string word)
{
	string searchURL = string(kHaiciQueryApi) + word;
    std::string answer = send_request(searchURL);
	return answer;
}

std::string Dict_Network::send_request(string &url)
{
	string buffer;
	CURL *curl;  
	CURLcode result;  
	// Create our curl handle
	curl = curl_easy_init();  
	if (curl)  
	{    
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);  
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());  
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);  
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);  

		result = curl_easy_perform(curl);  

		curl_easy_cleanup(curl);  

		if (result == CURLE_OK)  
		{  
			return buffer;
		}  
	}  
    buffer.clear();
	return buffer;
}
