#include <UnDI/Builder/ContainerBuilder.h>
#include <UnDI/Lifetime/ILifetimeScope.h>
#include <format>
#include <iostream>
#include <string_view>

using UN::IObject;
using UN::Object;
using UN::Ptr;

class ILogger : public IObject
{
public:
    UN_RTTI_Class(ILogger, "A521C28C-A4CD-491F-B89E-B666B57429BF");

    virtual void Log(const std::string_view& message) = 0;
};

class IDatabase : public IObject
{
public:
    UN_RTTI_Class(IDatabase, "CB0AA388-4D72-42AF-A735-6A41D8E2D143");

    virtual int LoadData() = 0;
};

class ITestService : public IObject
{
public:
    UN_RTTI_Class(ITestService, "5F9A04CD-AD84-4139-A7CE-BD5B22235BAF");

    virtual void Run() = 0;
};

class MyLogger : public Object<ILogger>
{
public:
    UN_RTTI_Class(MyLogger, "71963929-6C8A-42F5-B6E9-70F23510C71A");
    UN_Injectable(inline MyLogger)
    {
        std::cout << "Logger created!" << std::endl;
    }

    ~MyLogger() override
    {
        std::cout << "Logger deleted!" << std::endl;
    }

    inline void Log(const std::string_view& message) override
    {
        std::cout << message << std::endl;
    }
};

class MyDatabase : public Object<IDatabase>
{
    Ptr<ILogger> m_Logger;

public:
    UN_RTTI_Class(MyDatabase, "F1976DA2-FB38-4BF8-8F3E-9836A7713F06");
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

class TestService : public Object<ITestService>
{
    Ptr<ILogger> m_Logger;
    Ptr<IDatabase> m_Database;

public:
    UN_RTTI_Class(TestService, "64C8155D-67ED-4BF1-A475-5C095C7DCDD7");
    UN_Injectable(inline TestService, const Ptr<ILogger>& logger, const Ptr<IDatabase>& db)
        : m_Logger(logger)
        , m_Database(db)
    {
        m_Logger->Log("Logger injected in TestService!");
    }

    ~TestService() override
    {
        m_Logger->Log("TestService deleted!");
    }

    inline void Run() override
    {
        auto data = m_Database->LoadData();
        m_Logger->Log(std::format("Loaded data: {}", data));
    }
};

int main()
{
    UN::DI::ContainerBuilder builder{};
    builder.Bind<ILogger>().To<MyLogger>();
    builder.Bind<IDatabase>().To<MyDatabase>().InSingletonScope();
    builder.Bind<ITestService>().To<TestService>().InTransientScope();

    Ptr<UN::DI::IContainer> container = builder.Build();

    Ptr<ILogger> logger1 = container->Resolve<ILogger>().Unwrap();
    logger1->Log("Test message!");

    if (Ptr<UN::DI::ILifetimeScope> nestedScope = container->BeginScope().Unwrap())
    {
        Ptr<ILogger> logger2 = nestedScope->Resolve<ILogger>().Unwrap();
        logger2->Log("Test message!");

        Ptr<ILogger> logger3 = nestedScope->Resolve<ILogger>().Unwrap();
        logger3->Log("Test message!");
    }

    Ptr<ITestService> service = container->Resolve<ITestService>().Unwrap();
    service->Run();

    return 0;
}
