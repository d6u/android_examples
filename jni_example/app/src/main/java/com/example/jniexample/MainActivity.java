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
    }

    public native void callNativeTheOldWay();

    // instance native method will pass reference to the instance
    public native void callNativeTheNewWay();

    // static native method will pass reference to the class
    public static native void callNativeWithStaticMethod();

    public native String getStringFromJni(String name);
}