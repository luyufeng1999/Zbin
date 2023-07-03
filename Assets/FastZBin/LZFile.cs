using System;
using System.Runtime.InteropServices;

namespace FastZBin
{
    public class LZFile
    {
            #if UNITY_IOS && !UNITY_EDITOR
	        const string TRACK_DLL = "__Internal";
            #else
            const string TRACK_DLL = "zfile";
            #endif
            [DllImport(TRACK_DLL, EntryPoint = "LZOpen")]
            public static extern IntPtr Open(string zFilePath, string mode);
    
            [DllImport(TRACK_DLL, EntryPoint = "LZLoadHeader")]
            public static extern IntPtr LoadHeader(IntPtr zFileHandle);
    
            [DllImport(TRACK_DLL, EntryPoint = "LZGetBlockSize")]
            public static extern UInt64 GetBlockSize(IntPtr zFileHandle, int key, int key2);
    
            [DllImport(TRACK_DLL, EntryPoint = "LZGetBlock")]
            public static extern UInt64 Get(IntPtr zFileHandle, int key, int key2, byte[] buffer, int size);
    
            [DllImport(TRACK_DLL, EntryPoint = "LZClose")]
            public static extern int Close(IntPtr zFileHandle);
    }
}