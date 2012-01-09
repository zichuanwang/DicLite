#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "dict_client.h"
#include "dict_haici_xml_parser.h"
#include "dict_haici_xml_parser.h"

#define DICT_PORT       3845
#define MAX_BUFFER_SIZE 10000
#define QUEUE_SIZE      128

#define NETWORK_ONLY    4
#define VAGUE_SEARCH    2

typedef struct {
    int code;
} message_package;

typedef sockaddr_in socket_addr;

Dict_Client::Dict_Client() {
    
}

Dict_Client::~Dict_Client() {
    
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
                exit(0);
            }
            search_word(word);
            display_prompt("DicLite>");
        }
    }
    else if(_config.status() == normal) {
        search_word(_config.word());
    }
}

void Dict_Client::search_word(std::string &word) {
    int code = 0;
    if(_config.option().para[network_only] == true)
        code += NETWORK_ONLY;
    if(_config.option().para[vague_search] == true)
        code += VAGUE_SEARCH;
    send_message_to_server(code, word);
}

void Dict_Client::receive_answer(char* buffer) {
    
    Dict_Haici_XML_Parser parser;
    std::string xml_str = buffer;
    parser.parse(xml_str);
    
    if(_config.option().para[vague_search] == true)
        parser.display_key();
    if(_config.option().para[sentence]) {
        parser.display_sentence();
    }
    else 
        parser.display_std_info();
}

void Dict_Client::send_message_to_server(int code, std::string &msg)
{
	socket_addr client;
	memset(&client, 0, sizeof(client));
    
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = htons(INADDR_ANY);
	client.sin_port = htons(0);
    
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(client_socket < 0)
		exit(1);
    
	if(bind(client_socket, (struct sockaddr*)&client, sizeof(client)))
		exit(1);
    
	socket_addr server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(DICT_PORT);
    
	if(inet_aton("127.0.0.1", &server_addr.sin_addr) == 0) {
		printf("Server has not been set up!\n");
		exit(1);
	}
    
	socklen_t server_addr_length=sizeof(server_addr);
	if(connect(client_socket, (struct sockaddr*)&server_addr, server_addr_length) < 0) {
		printf("Can not connect to server!\n");
		exit(1);
	}
    
	static char buffer[MAX_BUFFER_SIZE];
    ((message_package *)&buffer[0])->code = code;
	strcpy(buffer + sizeof(message_package), msg.c_str());
	send(client_socket, buffer, MAX_BUFFER_SIZE, 0);
	size_t n;
	while((n = read(client_socket, buffer, MAX_BUFFER_SIZE)) > 0) {
		buffer[n] = '\0';
	}
    this->receive_answer(buffer);
	close(client_socket);
}