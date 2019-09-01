#include <gtest/gtest.h>
#include "TestCommunication.h"
#include "TestMessagesHandler.h"


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}