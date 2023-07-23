#include <UnDI/Container.h>
#include <format>
#include <iostream>
#include <string_view>

using UN::DI::IService;
using UN::DI::Ptr;

class ILogger : public IService
{
public:
    UN_CLASS_RTTI(ILogger, "A521C28C-A4CD-491F-B89E-B666B57429BF");

    virtual void Log(const std::string_view& message) = 0;
};

class IDatabase : public IService
{
public:
    UN_CLASS_RTTI(IDatabase, "CB0AA388-4D72-42AF-A735-6A41D8E2D143");

    virtual int LoadData() = 0;
};

class ITestService : public IService
{
public:
    UN_CLASS_RTTI(ITestService, "5F9A04CD-AD84-4139-A7CE-BD5B22235BAF");

    virtual void Run() = 0;
};

class MyLogger : public ILogger
{
public:
    UN_CLASS_RTTI(MyLogger, "71963929-6C8A-42F5-B6E9-70F23510C71A");
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
    UN_CLASS_RTTI(MyDatabase, "F1976DA2-FB38-4BF8-8F3E-9836A7713F06");
    UN_Injectable(inline explicit MyDatabase, const Ptr<ILogger>& logger)
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
    UN_CLASS_RTTI(TestService, "64C8155D-67ED-4BF1-A475-5C095C7DCDD7");
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
