using System.Runtime.InteropServices;
using System.Text;

namespace Native
{
    internal class NativeStringConvert
    {
        public static unsafe string GetString(byte* reference)
        {
            return Encoding.UTF8.GetString(MemoryMarshal.CreateReadOnlySpanFromNullTerminated(reference));
        }
        public static unsafe string GetString(byte* reference, int length)
        {
            return Encoding.UTF8.GetString(new Span<byte>(reference, length));
        }
        public static unsafe string GetString(sbyte* reference)
        {
            return Encoding.UTF8.GetString(MemoryMarshal.CreateReadOnlySpanFromNullTerminated((byte*)reference));
        }
        public static unsafe string GetString(sbyte* reference, int length)
        {
            return Encoding.UTF8.GetString(new Span<byte>(reference, length));
        }
        public static unsafe void SetString(sbyte* reference, int length, in ReadOnlySpan<char> source)
        {
            Span<byte> sbytes = new(reference, length);
            sbytes.Clear();
            Encoding.UTF8.GetBytes(source, sbytes);
        }

        public static unsafe void SetString(sbyte* reference, int length, in ReadOnlySpan<byte> source)
        {
            Span<byte> sbytes = new(reference, length);
            sbytes.Clear();
            source.CopyTo(sbytes);
        }
    }
}
