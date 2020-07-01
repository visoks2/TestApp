#include "TestCommunication.h"
#include "TestMessagesHandler.h"
#include <gtest/gtest.h>

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}