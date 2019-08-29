#pragma once 

#include <sys/types.h>


// debug common 
#define ENTRY 		std::cout << __FUNCTION__ << std::endl;
#define LINE_ENTRY  std::cout << __FUNCTION__ << " : " << __LINE__ << std::endl;

// common consts
constexpr timeval 			SELECT_TIMEOUT		{0, 250000};
