#include <UnDI/Container.h>
#include <format>
#include <iostream>
#include <string_view>

using UN::DI::IService;
using UN::DI::Ptr;

class ILogger : public IService
{
public:
    virtual void Log(const std::string_view& message) = 0;

    inline static int GetTypeId()
    {
        return 0;
    }
};

class IDatabase : public IService
{
public:
    virtual int LoadData() = 0;

    inline static int GetTypeId()
    {
        return 1;
    }
};

class ITestService : public IService
{
public:
    virtual void Run() = 0;

    inline static int GetTypeId()
    {
        return 2;
    }
};

class MyLogger : public ILogger
{
public:
    UN_Injectable(inline MyLogger) {}

    inline void Log(const std::string_view& message) override
    {
        std::cout << message << std::endl;
    }
};

class MyDatabase : public IDatabase
{
    Ptr<ILogger> m_Logger;

public:
    UN_Injectable(inline explicit MyDatabase, const Ptr<MyLogger>& logger)
        : m_Logger(logger)
    {
    }

    inline int LoadData() override
    {
        m_Logger->Log("Loaded data from DB!");
        return 123;
    }
};

class TestService : public ITestService
{
    Ptr<ILogger> m_Logger;
    Ptr<IDatabase> m_Database;

public:
    UN_Injectable(inline TestService, const Ptr<ILogger>& logger, const Ptr<IDatabase>& db)
        : m_Logger(logger)
        , m_Database(db)
    {
        m_Logger->Log("Logger injected in TestService!");
    }

    inline void Run() override
    {
        auto data = m_Database->LoadData();
        m_Logger->Log(std::format("Loaded data: {}", data));
    }
};

int main()
{
    UN::DI::Container container{};
    container.RegisterService<MyLogger, ILogger>();
    container.RegisterService<MyDatabase, IDatabase>();
    container.RegisterService<TestService, ITestService>();

    auto logger = container.GetService<ILogger>();
    logger->Log("Test message from logger!");

    auto db = container.GetService<IDatabase>();
    std::cout << db->LoadData() << std::endl;

    container.GetService<ITestService>()->Run();
    return 0;
}
