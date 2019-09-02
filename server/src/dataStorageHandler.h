#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <mutex>

#include <google/protobuf/util/delimited_message_util.h>

#include <data.pb.h>

class dataStorageHandler
{
    public:
        /**	
         * @brief 	dataStorageHandler c-tor
         */
        dataStorageHandler(/* args */);      
        /**	
         * @brief 	dataStorageHandler d-tor
         */
        ~dataStorageHandler();
        /**	
         * @brief 	Stores received record
         * 
         * @param   aRecord	    Record to store
        */
        void Store(gpb::Message_Record aRecord) ;
        /**	
         * @brief 	Reads received
         * 
         * @param   aRecord	    Record to read
        */
        std::string Read(gpb::Message_Record aRecord);
        /**	
         * @brief 	Updates record
         * 
         * @param   aRecord	    Record to update
        */
        void Update(gpb::Message_Record aRecord);
        /**	
         * @brief 	Removes record
         * 
         * @param   aRecord	    Record to remove
        */
        void Remove(gpb::Message_Record aRecord);
        /**	
         * @brief 	Authenticates record
         * 
         * @param   aRecord	    Record containing Authentication info
        */
        bool Authenticate(gpb::Message_Record aRecord);
    private:

        /**	
         * @brief 	All records in db file
        */
        std::vector<gpb::Message_Record> records;
        /**	
         * @brief 	A mutex 
        */
        std::mutex mutex;
};
