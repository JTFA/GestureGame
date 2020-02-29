package com.example.mediapipemultihandstrackingapp.view;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.DialogInterface;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
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

public class GameMap_close implements myCustomDelegate {
    public static String ctrId = "gamemap_close";
    public static int layout = R.layout.gamemap_close;
    public Activity activity;
    ImageView imgview;

    private int nowId = R.drawable.gamemap_close;
    private ArrayList<Integer> layouts = new ArrayList<>();

    public void setNowPos(int nowId) {
        this.nowId = nowId;
    }

    public GameMap_close(Activity activity, String name) {

        this.activity = activity;
        this.ctrId = name;
    }

    public boolean showMenu() {
        LibraryHelper.addCustomViewDelegate(layout, this);

        EngineWrapper.getInstance().queueEvent(() -> {
            float[] pos = {0f, 0f, 4f};
            float[] rot = {0f, 0f, 0f};
            float[] scale = {2.5f, 2.5f, 1f};
            int[] size = {540, 960};

            //MKitJniHelper.getInstance().removeCustomViewLayout(ctrId);
            //버그 수정 필요
            MKitJniHelper.getInstance().addCustomViewLayout(activity, ctrId, layout,
                    size, pos, rot, scale, 0.7f);
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
        android.util.Log.d("GameEngineCore", "onCreateAndAdd");

        com.kt.library.opengl.view.GLRelativeLayout layout = view.findViewById(R.id.gamemap_close);
        imgview = (ImageView) view.findViewById(R.id.gamemapciv);
        imgview.setImageResource(this.nowId);

        view.setOnTouchListener(new View.OnTouchListener(){
            @SuppressLint("ClickableViewAccessibility")
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        float X = event.getRawX();
                        float Y = event.getRawY();

                        Log.d("touchscreen", "X: "+X+"Y: "+Y);

                        //case 1
                        if(X<60 && Y<250) {
                            imgview.setImageResource(R.drawable.gc_1);
                            setNowPos(R.drawable.gc_1);
                        }
                        else if(X>60 && X<275 && Y<250) {
                            imgview.setImageResource(R.drawable.gc_2);
                            setNowPos(R.drawable.gc_2);
                        }
                        else if(X>300 && X<480 && Y<250) {
                            imgview.setImageResource(R.drawable.gc_3);
                            setNowPos(R.drawable.gc_3);
                        }
                        else if(X>500 && Y<250) {
                            imgview.setImageResource(R.drawable.gc_4);
                            setNowPos(R.drawable.gc_4);
                        }
                        else if(X>500 && Y>250 && Y<440) {
                            imgview.setImageResource(R.drawable.gc_5);
                            setNowPos(R.drawable.gc_5);
                        }
                        else if(X>500 && 440<Y && Y<650) {
                            imgview.setImageResource(R.drawable.gc_6);
                            setNowPos(R.drawable.gc_6);
                        }
                        else if(X>500 && 650<Y) {
                            imgview.setImageResource(R.drawable.gc_7);
                            setNowPos(R.drawable.gc_7);
                        }

                        else if(X>270 && X<480 && Y>640) {
                            imgview.setImageResource(R.drawable.gc_8);
                            setNowPos(R.drawable.gc_8);
                        }
                        else if(X>60 && X<270 && Y>640) {
                            imgview.setImageResource(R.drawable.gc_9);
                            setNowPos(R.drawable.gc_9);
                        }
                        else if(X<60 && Y>660) {
                            imgview.setImageResource(R.drawable.gc_10);
                            setNowPos(R.drawable.gc_10);
                        }

                        else if(X<60 && Y>440 && Y<660) {
                            imgview.setImageResource(R.drawable.gc_11);
                            setNowPos(R.drawable.gc_11);
                        }
                        else if(X<60 && Y>250 && Y<440) {
                            imgview.setImageResource(R.drawable.gc_12);
                            onHand(3000);
                        }
                }
                return true;
            }
        });


        return layout;
    }

    @Override
    public void onHand(int key){
        switch (key){
            case 3000:
                ArrayList<String> remove = new ArrayList<>();
                ArrayList<String> add = new ArrayList<>();
                remove.add("gamemap_close");
                add.add("end");
                ((MainActivity)MainActivity.mContext).removeandaddViews(remove, add, "end");
        }
    }

    @Override
    public void onHand(Vector3[] finger){}

    @Override
    public void onHand(String gesture){
        switch (gesture){

            case "zoomout":
                ArrayList<String> remove = new ArrayList<>();
                ArrayList<String> add = new ArrayList<>();

                remove.add("gamemap_close");
                add.add("gamemap");

                ((MainActivity)MainActivity.mContext).removeandaddViews(remove, add, "gamemap");

            case "flip":
//                ArrayList<String> remove_ = new ArrayList<>();
//                ArrayList<String> add_ = new ArrayList<>();
//
//                add_.add("card");
//                ((MainActivity)MainActivity.mContext).removeandaddViews(remove_, add_, "card");
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        activity.runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                imgview.setImageResource(R.drawable.gamemap_close_flip);
                            }
                        });
                    }
                }).start();
        }
    }

    @Override
    public void onDestroyAndRemove(View view, int layoutId) {
    }
}

