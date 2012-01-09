#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <syslog.h>

#include "dict_server.h"
#include "dict_database.h"
#include "dict_network.h"
#include "dict_haici_xml_parser.h"


using namespace std;

#define DICT_PORT 3845
#define MAX_BUFFER_SIZE 10000
#define QUEUE_SIZE 128

#define NETWORK_ONLY    4
#define VAGUE_SEARCH    2

typedef sockaddr_in socket_addr;

typedef struct {
    int code;
} message_package;

Dict_Server::Dict_Server() {}
Dict_Server::~Dict_Server() {}

bool Dict_Server::init_database()
{
	return database.init();
}

const void Dict_Server::init_daemon() {
	
	int pid = fork();
	if(pid > 0)
		exit(0);
	else if(pid < 0)
		exit(1);
	
	setsid();
	
	if(pid > 0)
		exit(0);
	else if(pid < 0)
		exit(1);
	
	for(int i = 0; i < sysconf(_SC_OPEN_MAX); i++) {
		close(i);
	}
	signal(SIGCHLD,SIG_IGN);
	umask(0);
}

int Dict_Server::run()
{
    int pid = fork();
    if(pid < 0) {
        printf("fork error\n");
        exit(1);
    }
    else if(pid > 0) {
        //printf("parent process\n");
        return 0;
    }
    
	init_daemon();
    	
	socket_addr server;
	memset(&server,0,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htons(INADDR_ANY);
	server.sin_port = htons(DICT_PORT);
	int server_socket = socket(AF_INET,SOCK_STREAM,0);
    
	if(server_socket < 0)
		return -1;
    
	if(bind(server_socket, (struct sockaddr*)&server, sizeof(server)))
		return -1;
	
	if(listen(server_socket,QUEUE_SIZE))
		return -1;
    
    if(this->init_database() == false)
        return -1;

	while(true)
	{
		struct sockaddr_in client;
		socklen_t length = sizeof(client);
		//syslog(LOG_INFO|LOG_LOCAL7, "in while");
		int new_server_socket = accept(server_socket, (struct sockaddr*)&client,&length);
        
		if(new_server_socket < 0)
			break;
        
		int child_pid = fork();
        if(child_pid < 0)
            break;
		else if(child_pid == 0)
		{
			//子进程
			char buffer[MAX_BUFFER_SIZE];
			memset(&buffer, 0, sizeof(buffer));
			length = (socklen_t)recv(new_server_socket, buffer, MAX_BUFFER_SIZE, 0);
            
            //syslog(LOG_INFO|LOG_LOCAL7, "DICT_SERVER:listening");
            
            int code = ((message_package *)&buffer[0])->code;
            const char* word = buffer + sizeof(message_package);
            std::string result = this->query_word(code, word);
            
            if(code == VAGUE_SEARCH || code == VAGUE_SEARCH + NETWORK_ONLY) {
                Dict_Haici_XML_Parser paser;
                std::string vauge_word = paser.get_vague_word(result);
                if(!vauge_word.empty())
                    result = this->query_word(code - VAGUE_SEARCH, vauge_word.c_str());
                    //result.clear();
            }
			write(new_server_socket, result.c_str(), strlen(result.c_str()));
			close(new_server_socket);
			exit(0);
		}
		else {
			//父进程
			close(new_server_socket);
		}
	}
    database.close();
    close(server_socket);
    exit(0);
}

std::string Dict_Server::query_word(int code, const char* word)
{
	char buffer[10000];
    
    if(code != NETWORK_ONLY && code != VAGUE_SEARCH + NETWORK_ONLY)
        if(database.search(word,buffer) == SEARCH_SUCCESS)
            return std::string(buffer);
    
    std::string result = network.get_word_from_net(std::string(word));
    database.save(word, result.c_str());
    database.refresh();
    
    return result;
    
}
