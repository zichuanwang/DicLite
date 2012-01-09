#include <iostream>
#include "dict_database.h"

#define DICT_DB_FILE_PATH "/tmp/diclite_database"

Dict_Database::Dict_Database() {
    _p_db = NULL;
}

Dict_Database::~Dict_Database() {
    if(_p_db != NULL)
        dbm_close(_p_db);
    _p_db = NULL;
}

bool Dict_Database::init() {
    if(this->open_database() == false) {
        return false;
    }
    return true;
}

void Dict_Database::close() {
    dbm_close(_p_db);
    _p_db = NULL;
}

bool Dict_Database::open_database() {
	_p_db = dbm_open(DICT_DB_FILE_PATH, O_RDWR | O_CREAT, 0666);
	if(!_p_db) {
		return false;	
	}
    return true;
}

int Dict_Database::save(char* key, char* value) {
    datum datum_key, datum_value;
    
    datum_key.dptr = key;
    datum_key.dsize = strlen(key);
    
    datum_value.dptr = value;
    datum_value.dsize = strlen(value) + 1;
    
    int result = dbm_store(this->_p_db, datum_key, datum_value, DBM_REPLACE);
    return result;
}

int Dict_Database::save(const char* key,const char* value) {
    return save(const_cast<char *>(key), const_cast<char *>(value));
}

int Dict_Database::search(char *key, char* buffer) {
    datum datum_key, datum_value;
    
    datum_key.dptr = key;
    datum_key.dsize = strlen(key);
    
    datum_value = dbm_fetch(this->_p_db, datum_key);
    if(datum_value.dptr) {
        memcpy(buffer, datum_value.dptr, datum_value.dsize);
        return SEARCH_SUCCESS;
    }
    return SEARCH_FAILURE;
}

int Dict_Database::search(const char* key, char* buffer) {
    return search(const_cast<char*>(key), buffer);
}

void Dict_Database::refresh() {
    this->close();
    this->init();
}

