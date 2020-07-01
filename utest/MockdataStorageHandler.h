#include "dataStorageHandler.h"
#include <gmock/gmock.h> // Brings in gMock.

class MockdataStorageHandler : public dataStorageHandler
{
  public:
    MOCK_METHOD1(Store, void(gpb::Message_Record));
    MOCK_METHOD1(Read, std::string(gpb::Message_Record));
    MOCK_METHOD1(Update, void(gpb::Message_Record));
    MOCK_METHOD1(Remove, void(gpb::Message_Record));
    MOCK_METHOD1(Authenticate, bool(gpb::Message_Record));
};