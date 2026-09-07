using ManagedModule.Interop;

Guid MODULE_GUID = new("12345678-1234-1234-1234-123456789abc");

using EmsModule module = new()
{
    Author = "Hellodda",
    Name = "System Explorer Managed Extension",
    Description = "bla bla bla",
    Guid = MODULE_GUID,
};

if (module.RegisterModule())
{
    Console.WriteLine("Success: Module registered successfully");
}
else
{
    Console.WriteLine("Error: Module registration failed");
}