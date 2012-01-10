//
//  main.cpp
//  dict
//
//  Created by Blue Bitch on 12-1-8.
//  Copyright (c) 2012年 TJU. All rights reserved.
//

#include "dict_input.h"
#include "dict_client.h"

Dict_Client client;

void sigroutine(int dunno) {
    switch(dunno) { 
        case SIGINT:  
            printf("\nClose database safely\n");
            client.stop();
            exit(0);
    }
}

int main(int argc, char* argv[]) {
    
    Dict_Input input;
    input.resolve_para(argc, argv);
    if(input.has_error()) {
        input.display_usage();
        exit(1);
    }
    
    signal(SIGINT, sigroutine); 
    
    client.init(input);
    client.run();
    
    exit(0);
}
