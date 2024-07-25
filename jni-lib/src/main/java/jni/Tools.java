package jni;

import java.util.List;

public class Tools {
    static {
        if (!LibraryLoader.load(Tools.class, "tools"))
            System.loadLibrary("tools");
    }

    public native List<DnodeAttributes> getFailedChunks(String poolName);

    public native void writeRepairData(String poolName, DnodeAttributes dnode, int stripeIdx, int colIdx, byte[] data);
}