package jni;

import java.util.List;

public class DnodeAttributes {
    
    public int objset;
    public int object;
    public int type;
    public String path;
    public int fsize;
    public int numStripes;
    public int numRemainderStripes;
    public int dcols;
    public int nparity;
    public List<Integer> childStatus;

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("objset: ").append(objset).append("\n");
        sb.append("object: ").append(object).append("\n");
        sb.append("type: ").append(type).append("\n");
        sb.append("path: ").append(path).append("\n");
        sb.append("fsize: ").append(fsize).append("\n");
        sb.append("numStripes: ").append(numStripes).append("\n");
        sb.append("numRemainderStripes: ").append(numRemainderStripes).append("\n");
        sb.append("dcols: ").append(dcols).append("\n");
        sb.append("nparity: ").append(nparity).append("\n");
        sb.append("childStatus: ").append(childStatus).append("\n");
        return sb.toString();
    }

    public int numFailedCols() {
        int failedCols = 0;
        for (int status : childStatus) {
            if (status != 0) {
                failedCols++;
            }
        }
        return failedCols;
    }

}
