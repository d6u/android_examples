package com.example.jniexample;

public class HybridObject {

    private static int counter = 0;

    public HybridObject() {
        mId = counter;
        counter++;
        init(mId);
    }

    private final int mId;

    private native void init(int id);

    public native void sayId();

    private int getId() {
        return mId;
    }
}
