package com.example.mediapipemultihandstrackingapp.view;

import android.app.Activity;
import android.content.DialogInterface;
import android.view.View;
import android.view.WindowManager;
import android.widget.Adapter;
import android.widget.AdapterView;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.Toast;

import com.example.mediapipemultihandstrackingapp.ImageAdapter;
import com.example.mediapipemultihandstrackingapp.MainActivity;
import com.example.mediapipemultihandstrackingapp.R;
import com.example.mediapipemultihandstrackingapp.Vector3;
import com.example.mediapipemultihandstrackingapp.graphics.EngineWrapper;
import com.example.mediapipemultihandstrackingapp.myCustomDelegate;
import com.kt.library.opengl.view.CustomViewDelegate;
import com.kt.library.opengl.view.IGLLayout;
import com.kt.util.LibraryHelper;
import com.maxst.mkit.MKitJniHelper;
import android.util.DisplayMetrics;

import java.util.ArrayList;

import static java.security.AccessController.getContext;

public class GameMap implements myCustomDelegate {
    public static String ctrId = "gamemap";
    public static int layout = R.layout.gamemap;
    public Activity activity;
    public ImageView imageview;

    public GameMap(Activity activity, String name) {

        this.activity = activity;
        this.ctrId = name;
    }

    public boolean showMenu() {
        LibraryHelper.addCustomViewDelegate(layout, this);

        EngineWrapper.getInstance().queueEvent(() -> {
            float[] pos = {0f, 0f, 5f};
            float[] rot = {0f, 50f, 0f};
            float[] scale = {2f, 2f, 1f};
            int[] size = {540, 960};

            //MKitJniHelper.getInstance().removeCustomViewLayout(ctrId);
            //버그 수정 필요
            MKitJniHelper.getInstance().addCustomViewLayout(activity, ctrId, layout,
                    size, pos, rot, scale, 0.7f);


//            MKitJniHelper.getInstance().addImageAndAlpha(ctrId2, "res/hdpi-png/label_bg.png",
//                    pos[0], pos[1], pos[2],
//                    rot[0], rot[1], rot[2],
//                    scale[0], scale[1], scale[2],0.7F);
        });

        return true;
    }

    public void removeMenu() {
        EngineWrapper.getInstance().queueEvent(() -> {
            MKitJniHelper.getInstance().removeCustomViewLayout(ctrId);
        });
    }


    @Override
    public IGLLayout onCreateAndAdd(View view, int layoutId) {
        android.util.Log.d("GameEngineCore", "onCreateAndAdd");

        com.kt.library.opengl.view.GLRelativeLayout layout = view.findViewById(R.id.gamemap);

        return layout;
    }

    @Override
    public void onHand(int key){
        switch (key){
            case 31:
                ArrayList<String> remove = new ArrayList<>();
                ArrayList<String> add = new ArrayList<>();
                add.add("mainpage");

                ((MainActivity)MainActivity.mContext).removeandaddViews(remove, add, "mainpage");
        }
    }

    @Override
    public void onHand(Vector3[] finger){}

    @Override
    public void onHand(String gesture){
    }

    @Override
    public void onDestroyAndRemove(View view, int layoutId) {
    }
}

