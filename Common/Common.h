#pragma once 

#include <sys/types.h>
#include <iostream>
#include <string>


// debug common 
#define ENTRY 		        std::cout << __FUNCTION__ << '\n';
#define LINE_ENTRY          std::cout << __FUNCTION__ << " : " << __LINE__ << '\n';
#define DEBUG_LOG(m)        std::cout << '[' << __FUNCTION__ << "]\t\t\t"  << #m << " : " << m << '\n';
#define DEBUG_LOG_MSG(m, msg)   std::cout << '[' << __FUNCTION__ << "]\t"  << msg << '\t' << #m << " : " << m << '\n';

// common consts
constexpr   timeval         SELECT_TIMEOUT		        {0, 250000};
constexpr   const char *    MASTER_PASSWORD		    = "verySecurePassword";
constexpr   const char *    DB_STORAGE_FILE_NAME    = "db.txt";