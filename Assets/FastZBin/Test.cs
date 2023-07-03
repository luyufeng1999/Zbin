using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using FastZBin;
using UnityEngine;

public class Test : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        //byte[] data = BetterStreamingAssets.ReadAllBytes("buff.csv.zbin");
        //File.WriteAllBytes(Application.persistentDataPath + "/buff.csv.zbin", data);
        
        IntPtr zFile = ZFile.Open(Application.persistentDataPath + "/buff.csv.zbin", "rb");
        ZFile.LoadHeader(zFile);
        UInt64 size = ZFile.GetBlockSize(zFile, 1, 1);
        Debug.Log($"BlockSize: {size}");
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
