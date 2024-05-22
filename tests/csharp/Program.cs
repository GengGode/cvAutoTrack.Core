global using Windows.Win32.Foundation.Metadata;

namespace csharp
{
    internal class Program
    {
        static void Main(string[] args)
        {
            unsafe
            {
                var core = cvAutoTrack.Core.Native.Methods.CreateInstance();
                core->init();
                core->start();
                core->stop();
                core->release();
                core->destroy(core);

            }
        }
    }
}
