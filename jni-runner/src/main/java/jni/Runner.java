package jni;

import java.util.List;

public class Runner {
    public static void main(String[] args) {
        Tools tools = new Tools();
        
        List<DnodeAttributes> atrributes = tools.getFailedChunks("pool");
        System.out.println("Attribute size " + atrributes.size());
        
        // print dnode
        DnodeAttributes repairDnode = null;
        for (DnodeAttributes dnode : atrributes) {
            repairDnode = dnode;
            System.out.println("objset: " + dnode.objset);
            System.out.println("object: " + dnode.object);
            System.out.println("type: " + dnode.type);
            System.out.println("path: " + dnode.path);
            System.out.println("fsize: " + dnode.fsize);
            System.out.println("numStripes: " + dnode.numStripes);
            System.out.println("numRemainderStripes: " + dnode.numRemainderStripes);
            System.out.println("dcols: " + dnode.dcols);
            System.out.println("nparity: " + dnode.nparity);
            System.out.println("childStatus: " + dnode.childStatus);
            System.out.println("-------");
        }

        System.out.println("====================");

        // Write the repair data
        assert repairDnode != null;
        String testData = "Hello World!";
        tools.writeRepairData("pool", repairDnode, 0, 0, testData.getBytes());
    }
}