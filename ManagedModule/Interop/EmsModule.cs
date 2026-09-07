using Microsoft.Win32.SafeHandles; 

namespace ManagedModule.Interop
{
    internal class EmsModule : IDisposable
    {
        private SafeFileHandle? _handle;

        public Guid Guid { get; set; }
        public string Name { get; set; } = "";
        public string Author { get; set; } = "";
        public string Description { get; set; } = "";

        public bool RegisterModule()
        {
            EMSM_REGISTER_MODULE registerModuleMessage = new()
            {
                ModuleGuid = Guid,
                ModuleDescription = Description,
                ModuleName = Name,
                ModuleAuthor = Author
            };

            bool isSuccess = EmsNativeApi.EmsConnectToHost(out IntPtr nativeHandle, ref registerModuleMessage) >= 0;

            if (isSuccess)
            {
                _handle = new SafeFileHandle(nativeHandle, ownsHandle: true);
            }
            return isSuccess;
        }

        public void Dispose()
        {
            _handle?.Dispose();
        }
    }
}