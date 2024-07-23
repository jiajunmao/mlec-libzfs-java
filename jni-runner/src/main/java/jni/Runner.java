package jni;

import java.util.List;

public class Runner {
    public static void main(String[] args) {
        Tools tools = new Tools();
        
        List<DnodeAttributes> atrributes = tools.getFailedChunks("pool", 54);
        System.out.println("Attribute size " + atrributes.size());
        System.out.println(atrributes.get(0).path);
    }
}