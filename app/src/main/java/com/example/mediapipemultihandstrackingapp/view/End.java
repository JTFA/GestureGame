package com.example.mediapipemultihandstrackingapp.view;

import android.app.Activity;
import android.view.View;
import android.widget.Button;

import com.example.mediapipemultihandstrackingapp.MainActivity;
import com.example.mediapipemultihandstrackingapp.R;
import com.example.mediapipemultihandstrackingapp.Vector3;
import com.example.mediapipemultihandstrackingapp.graphics.EngineWrapper;
import com.example.mediapipemultihandstrackingapp.myCustomDelegate;
import com.kt.library.opengl.view.IGLLayout;
import com.kt.util.LibraryHelper;
import com.maxst.mkit.MKitJniHelper;

import java.util.ArrayList;

public class End implements myCustomDelegate {

    float[] pos = {0f, 0f, 5f};
    float[] rot = {0f, 0f, 0f};
    float[] scale = {1f, 1f, 1f};

    public static String ctrId = "end";
    public static int layout = R.layout.endbutton;
    public Activity activity;
    Button button;

    public End(Activity activity, String name) { this.activity = activity; this.ctrId = name;}

    @Override
    public boolean showMenu(){
        LibraryHelper.addCustomViewDelegate(layout, this);

        EngineWrapper.getInstance().queueEvent(() -> {
            float[] pos = {0f, 0f, 5f};
            float[] rot = {0f, 0f, 0f};
            float[] scale = {3f, 3f, 1f};
            int[] size = {540, 960};
            //버그 수정 필요
            MKitJniHelper.getInstance().addCustomViewLayout(activity, ctrId, layout,
                    size, pos, rot, scale, 1f);
        });
        return true;
    }

    @Override
    public void removeMenu() {
        EngineWrapper.getInstance().queueEvent(()->{
            MKitJniHelper.getInstance().removeCustomViewLayout(ctrId);
        });
    }

    public IGLLayout onCreateAndAdd(View view, int layoutId) {
        android.util.Log.d("END", "onCreateAndAdd");
        com.kt.library.opengl.view.GLRelativeLayout layout = view.findViewById(R.id.endbutton);

        button = (Button)layout.findViewById(R.id.button_);

        button.setOnClickListener((View view1) -> {
        });

        return layout;
    }

    @Override
    public void onHand(int key){
    }

    @Override
    public void onHand(Vector3[] fingerNow){

    }

    @Override
    public void onHand(String gesture){

    }

    @Override
    public void onDestroyAndRemove(View view, int layoutId) {
    }
}
