#pragma once 

#include <sys/types.h>
#include <iostream>
#include <string>


// debug common 
#define ENTRY 		        std::cout << __FUNCTION__ << '\n';
#define LINE_ENTRY          std::cout << __FUNCTION__ << " : " << __LINE__ << '\n';
#define DEBUG_LOG(m)        std::cout << '[' << __FUNCTION__ << "]\t\t\t"  << #m << " : " << m << '\n';
#define DEBUG_LOG_MSG(m, msg)   std::cout << '[' << __FUNCTION__ << "]\t"  << msg << '\t' << #m << " : " << m << '\n';

// TODO: add some kind of configuration manager 
constexpr   const char *    DB_STORAGE_FILE_NAME    = "db.txt";
constexpr   const char *    MASTER_PASSWORD		    = "verySecurePassword";
constexpr   const char *    DEFAULT_IP              = "127.0.0.1";
constexpr   int             DEFAULT_PORT            = 5050;
constexpr   timeval         SELECT_TIMEOUT		        {0, 250000};