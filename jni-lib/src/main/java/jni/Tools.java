package jni;

import java.util.List;

public class Tools {
    static {
        if (!LibraryLoader.load(Tools.class, "tools"))
            System.loadLibrary("tools");
    }

    public native List<DnodeAttributes> getFailedChunks(String poolName, int objsetId);

    public native void writeRepairData(DnodeAttributes dnode, int stripeId, int colIdx, byte[] data);
}