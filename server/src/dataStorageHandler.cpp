#include "dataStorageHandler.h"
#include "Common.h"
dataStorageHandler::dataStorageHandler()
{
    std::string line;
    std::ifstream myfile (DB_STORAGE_FILE_NAME);
    if (myfile.is_open())
    {
        gpb::Message_Record msg;
        bool clean_eof;
        google::protobuf::io::IstreamInputStream is{&myfile};
        while (google::protobuf::util::ParseDelimitedFromZeroCopyStream(&msg, &is, &clean_eof))
        {
            records.push_back(msg);
        }
        myfile.close();
    }
}

dataStorageHandler::~dataStorageHandler()
{
    std::ofstream newFile(DB_STORAGE_FILE_NAME);

    if(newFile.is_open()) {
        for (auto &&rec : records) {
            google::protobuf::util::SerializeDelimitedToOstream(rec, &newFile);
        }
    }
    else {
        // You're in trouble now Mr!
    }
    newFile.close();
}

void dataStorageHandler::Store(gpb::Message_Record aRecord) {
    std::lock_guard<std::mutex> lock(mutex);

    records.push_back(aRecord);
}
std::string dataStorageHandler::Read(gpb::Message_Record aRecord) {
    std::lock_guard<std::mutex> lock(mutex);
    
    std::string idToRead      ( aRecord.id()       );
    std::stringstream result;
    for (auto &&rec : records) {
        google::protobuf::util::SerializeDelimitedToOstream(rec, &result);
    }
    return result.str();
}
void dataStorageHandler::Update(gpb::Message_Record aRecord) {
    std::lock_guard<std::mutex> lock(mutex);

    std::string idToUpdate      ( aRecord.id()       );
    std::string userToUpdate    ( aRecord.username() );
    auto rec = std::find_if(records.begin(), records.end(), [idToUpdate, userToUpdate](gpb::Message_Record& aRec) {
                                                            return aRec.id()        == idToUpdate 
                                                                && aRec.username()  == userToUpdate;
                                                        });
    if (rec == records.end()){
        // TODO: record not found -> store???
        records.push_back(aRecord);
    } else {
        records.erase(rec);
        records.push_back(aRecord);
    }
}
void dataStorageHandler::Remove(gpb::Message_Record aRecord) {
    std::lock_guard<std::mutex> lock(mutex);

    std::string idToDelete      ( aRecord.id()       );
    std::string userToDelete    ( aRecord.username() );
    auto rec = std::find_if(records.begin(), records.end(), [idToDelete, userToDelete](gpb::Message_Record& aRec) {
                                                            return aRec.id()        == idToDelete 
                                                                && aRec.username()  == userToDelete;
                                                        });
    records.erase(rec);
}
bool dataStorageHandler::Authenticate(gpb::Message_Record aRecord) {
	DEBUG_LOG(aRecord.password() );

    std::string psw    ( aRecord.password() );
    return psw.compare(MASTER_PASSWORD) == 0;
}
