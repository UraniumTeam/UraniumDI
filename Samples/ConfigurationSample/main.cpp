#include <UnDI/Config/ConfigurationBuilder.h>
#include <UnDI/Config/Reg/CommandLineConfigurationRegistry.h>
#include <UnDI/Config/Reg/MemoryConfigurationRegistry.h>
#include <iostream>

using namespace UN;
using namespace UN::DI;

int main(int argc, char** argv)
{
    Ptr registry = AllocateObject<MemoryConfigurationRegistry>();
    registry->Add("test-key", "test-value");
    registry->Add("test-key-1", "test-value-1");
    registry->Add("test-key:test-nested", "test-value-2");

    Ptr commandLine = AllocateObject<CommandLineConfigurationRegistry>(argc, argv);

    Ptr config = ConfigurationBuilder{}.AddRegistry(registry.Get()).AddRegistry(commandLine.Get()).Build();

    // These value can be overridden by command-line arguments
    std::cout << config->Get("test-key").value() << std::endl;
    std::cout << config->Get("test-key-1").value() << std::endl;
    std::cout << config->Get("test-key:test-nested").value() << std::endl;

    auto* section = config->GetSection("test-key:test-nested");
    std::cout << "Section at: " << section->GetPath() << std::endl;
    std::cout << "Value: " << section->GetValue().value() << std::endl;
}
