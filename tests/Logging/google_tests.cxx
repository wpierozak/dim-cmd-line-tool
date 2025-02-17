#include<Logging/Logger.hxx>
#include<gtest/gtest.h>

TEST(Logger, Debug)
{
    EXPECT_NO_THROW(LOG(Logger::Mode::Debug) << "Debug message");
}

TEST(Logger, Info)
{
    EXPECT_NO_THROW(LOG(Logger::Mode::Info) << "Info message");
}

TEST(Logger, Warning)
{
    EXPECT_NO_THROW(LOG(Logger::Mode::Warning) << "Warning message");
}

TEST(Logger, Error)
{
    EXPECT_NO_THROW(LOG(Logger::Mode::Error) << "Error message");
}
