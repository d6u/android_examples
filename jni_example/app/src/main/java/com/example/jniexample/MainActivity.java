package com.example.jniexample;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.example.jniexample.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'jniexample' library on application startup.
    static {
        System.loadLibrary("jniexample");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(getStringFromJni("World"));

        HybridObject one = new HybridObject();
        HybridObject two = new HybridObject();

        two.sayId();
        one.sayId();
    }

    // It doesn't matter if Java methods are private or public.
    private native void callNativeTheOldWay();

    // instance native method will pass reference to the instance
    private native void callNativeTheNewWay();

    // static native method will pass reference to the class
    private static native void callNativeWithStaticMethod();

    // Native side can also register a static method of a class to to handle the call
    private static native void callNativeStaticMethodWithStaticMethod();

    private native String getStringFromJni(String name);

    private String applyPrefix(String str) {
        return "Hello, " + str;
    }

    private static String applySuffix(String str) {
        return str + "!";
    }
}