# UraniumDI
UraniumDI is a dependency injection library for modern C++.

### Example
1. Write your interfaces and implementations:
    ```cpp
    // All service interfaces must derive from IObject
    class ILogger : public UN::IObject
    {
    public:
        // And declare RTTI with a macro, UUID here is used to identify the type
        UN_RTTI_Class(ILogger, "A521C28C-A4CD-491F-B89E-B666B57429BF");
    
        virtual void Log(const std::string_view& message) = 0;
    };
    
    // Implementations must derive from Object<IInterface>
    class MyLogger : public UN::Object<ILogger>
    {
    public:
        UN_RTTI_Class(MyLogger, "71963929-6C8A-42F5-B6E9-70F23510C71A");
    
        inline MyLogger() {}
    
        inline void Log(const std::string_view& message) override
        {
            std::cout << message << std::endl;
        }
    };
    ```

2. Register your services within an IoC container:
    ```cpp
    UN::DI::ContainerBuilder builder{};
    builder.Bind<ILogger>().To<MyLogger>();
    builder.Bind<IDatabase>().To<MyDatabase>().InSingletonScope();
    
    Ptr container = builder.Build();
    ```

3. And resolve them:
    ```cpp
    UN::Ptr logger = container->Resolve<ILogger>().Unwrap();
    logger->Log("Message from logger!");
    
    UN::Ptr db = container->Resolve<IDatabase>().Unwrap();
    logger->Log(std::format("Data loaded: {}", db->LoadData()));
    ```
