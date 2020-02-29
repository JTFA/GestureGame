package com.example.mediapipemultihandstrackingapp.view;

import android.app.Activity;
import android.view.View;
import android.widget.ImageView;

import com.example.mediapipemultihandstrackingapp.MainActivity;
import com.example.mediapipemultihandstrackingapp.R;
import com.example.mediapipemultihandstrackingapp.Vector3;
import com.example.mediapipemultihandstrackingapp.graphics.EngineWrapper;
import com.example.mediapipemultihandstrackingapp.myCustomDelegate;
import com.kt.library.opengl.view.IGLLayout;
import com.kt.util.LibraryHelper;
import com.maxst.mkit.MKitJniHelper;

import java.util.ArrayList;

public class Card implements myCustomDelegate {

    public static String ctrId = "card";
    ///////////////////////////////////////////////////////rename card->card
    public static int layout = R.layout.card;
    public Activity activity;
    ImageView imageview;

    public Card(Activity activity, String name) { this.activity = activity; this.ctrId = name;}

    public boolean showMenu() {
        LibraryHelper.addCustomViewDelegate(layout, this);

        EngineWrapper.getInstance().queueEvent(() -> {
            float[] pos = {0f, 0f, 4f};
            float[] rot = {0f, 0f, 0f};
            float[] scale = {2.5f, 2.5f, 1f};
            int[] size = {540, 960};

            //버그 수정 필요
            MKitJniHelper.getInstance().addCustomViewLayout(activity, ctrId, layout,
                    size, pos, rot, scale, 1f);
        });

        return true;
    }

    public void removeMenu() {
        EngineWrapper.getInstance().queueEvent(()->{
            MKitJniHelper.getInstance().removeCustomViewLayout(ctrId);
        });
    }


    @Override
    public IGLLayout onCreateAndAdd(View view, int layoutId) {
        com.kt.library.opengl.view.GLRelativeLayout layout = view.findViewById(R.id.card);
        imageview = (ImageView) view.findViewById(R.id.cardiv);
        /////////////////////////////////////////////////////////////////
        // imageview.setImageResource(R.drawable.card);
        //imageview.setX//setY

        return layout;
    }

    @Override
    public void onHand(int key){
        switch (key) {

        }
    }


    @Override
    public void onHand(Vector3[] fingersNow){
    }

    @Override
    public void onHand(String gesture){
        switch (gesture){

            case "zoomout":
                ArrayList<String> remove = new ArrayList<>();
                ArrayList<String> add = new ArrayList<>();

                remove.add("gamemap_close"); remove.add("card");
                add.add("gamemap");

                ((MainActivity)MainActivity.mContext).removeandaddViews(remove, add, "gamemap");

        }
    }

    @Override
    public void onDestroyAndRemove(View view, int layoutId) {
    }
}