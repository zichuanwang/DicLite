#pragma once
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
//#include <gdbm.h>
#include <ndbm.h>


#define SEARCH_SUCCESS  0
#define SEARCH_FAILURE  1

#define SAVE_SUCCESS    0

class Dict_Database {
private:
    //GDBM_FILE _p_db;
    DBM *_p_db;
public:
    Dict_Database();
    virtual ~Dict_Database();
    bool init();
    void close();
    void refresh();
    int save(char* key, char* value);
    int save(const char* key,const char* value);
    int search(char *key, char* buffer);
    int search(const char *key, char* buffer);
protected:
    bool open_database();
};
