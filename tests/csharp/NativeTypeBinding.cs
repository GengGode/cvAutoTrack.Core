using System.Runtime.InteropServices;

namespace Windows.Win32.Foundation.Metadata
{
    [StructLayout(LayoutKind.Auto, CharSet = CharSet.Auto)]
    [AttributeUsage(AttributeTargets.Struct | AttributeTargets.Enum | AttributeTargets.Property | AttributeTargets.Field | AttributeTargets.Parameter | AttributeTargets.ReturnValue, AllowMultiple = false, Inherited = false)]
    [ComVisible(true)]
    public sealed class NativeTypeNameAttribute : Attribute
    {
        public extern NativeTypeNameAttribute(string Name);
    }
}
