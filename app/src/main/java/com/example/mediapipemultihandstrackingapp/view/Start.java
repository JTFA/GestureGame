package com.example.mediapipemultihandstrackingapp.view;

import android.app.Activity;
import android.content.Intent;
import android.opengl.GLSurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.Toast;

import com.example.mediapipemultihandstrackingapp.MainActivity;
import com.example.mediapipemultihandstrackingapp.R;
import com.example.mediapipemultihandstrackingapp.Vector3;
import com.example.mediapipemultihandstrackingapp.graphics.EngineWrapper;
import com.example.mediapipemultihandstrackingapp.myCustomDelegate;
import com.kt.library.opengl.view.CustomViewDelegate;
import com.kt.library.opengl.view.IGLLayout;
import com.kt.util.LibraryHelper;
import com.maxst.mkit.MKitJni;
import com.maxst.mkit.MKitJniHelper;

import java.util.ArrayList;

import static com.maxst.ar.MaxstAR.getApplicationContext;

public class Start implements myCustomDelegate {

    float[] pos = {0f, 0f, 5f};
    float[] rot = {0f, 0f, 0f};
    float[] scale = {1f, 1f, 1f};

    public static String ctrId = "start";
    public static int layout = R.layout.startbutton;
    public Activity activity;
    Button button;

    //private artestlayout = (ARTestLayout)layout.findViewById(R.id.button);
    public Start(Activity activity, String name) { this.activity = activity; this.ctrId = name;}

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
        android.util.Log.d("StartButton", "onCreateAndAdd");
        com.kt.library.opengl.view.GLRelativeLayout layout = view.findViewById(R.id.startbutton);

        button = (Button)layout.findViewById(R.id.button);

        button.setOnClickListener((View view1) -> {
            //((MainActivity)MainActivity.mContext).changeMenu(ctrId, "gamemap");
            //((MainActivity)MainActivity.mContext).addLayer("card", false);
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
        switch (gesture){
            case "click":
                ArrayList<String> remove = new ArrayList<>();
                ArrayList<String> add = new ArrayList<>();
                remove.add(ctrId);
                add.add("gamemap");

                ((MainActivity)MainActivity.mContext).removeandaddViews(remove, add, "gamemap");
        }

    }

    @Override
    public void onDestroyAndRemove(View view, int layoutId) {
    }
}
