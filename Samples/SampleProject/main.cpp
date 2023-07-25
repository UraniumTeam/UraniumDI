#include <UnDI/Registry/ServiceRegistryBuilder.h>
#include <UnDI/IContainer.h>
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
    UN_Injectable(inline MyLogger) {}

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

    inline void Run() override
    {
        auto data = m_Database->LoadData();
        m_Logger->Log(std::format("Loaded data: {}", data));
    }
};

int main()
{
    UN::DI::ServiceRegistryBuilder registryBuilder{};
    registryBuilder.Bind<ILogger>().To<MyLogger>().InSingletonScope();
    auto* registry = registryBuilder.Build();
    (void)registry;
    return 0;
}
