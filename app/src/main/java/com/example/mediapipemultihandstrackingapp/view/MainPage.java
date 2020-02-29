package com.example.mediapipemultihandstrackingapp.view;

import android.app.Activity;
import android.media.AudioManager;
import android.media.Image;
import android.media.SoundPool;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.example.mediapipemultihandstrackingapp.MainActivity;
import com.example.mediapipemultihandstrackingapp.R;
import com.example.mediapipemultihandstrackingapp.Vector3;
import com.example.mediapipemultihandstrackingapp.graphics.EngineWrapper;
import com.example.mediapipemultihandstrackingapp.myCustomDelegate;
import com.google.common.util.concurrent.ThreadFactoryBuilder;
import com.kt.library.opengl.view.CustomViewDelegate;
import com.kt.library.opengl.view.IGLLayout;
import com.kt.util.LibraryHelper;
import com.maxst.mkit.MKitJniHelper;

import java.util.ArrayList;
import java.util.Random;
import java.util.logging.Handler;

public class MainPage implements myCustomDelegate {

    public static String ctrId = "mainpage";
    public static int layout = R.layout.mainpage;
    public Activity activity;
    ImageView imageview;
    public int dice_num = 0;
    //public boolean visibility = true;

    int images[] = new int[6];

    //public void setvis(boolean visibility) { this.visibility = visibility;}

    public MainPage(Activity activity, String name) { this.activity = activity; this.ctrId = name;}

    public void setrandnum(){
        this.dice_num = new Random().nextInt(5);
    }

    public int getDice_num() {
        return this.dice_num+1;
    }

    public int setandget_dnum(){
        this.dice_num = new Random().nextInt(5);
        return this.dice_num+1;
    }

    public boolean showMenu() {
        LibraryHelper.addCustomViewDelegate(layout, this);

        EngineWrapper.getInstance().queueEvent(() -> {
            float[] pos = {0f, 0f, 2f};
            float[] rot = {0f, 0f, 0f};
            float[] scale = {0.5f, 0.5f, 0.5f};
            int[] size = {540, 960};

        //MKitJniHelper.getInstance().removeCustomViewLayout(ctrId);
        //버그 수정 필요
        MKitJniHelper.getInstance().addCustomViewLayout(activity, ctrId, layout,
               size, pos, rot, scale, 1f);
        setrandnum();
        Toast.makeText(activity, Integer.toString(getDice_num()+1), Toast.LENGTH_SHORT).show();

        //그냥 test
//        MKitJniHelper.getInstance().addImageAndAlpha(ctrId, "res/mipmap-hdpi/ic_launcher.png",
//                pos[0], pos[1], pos[2],
//                rot[0], rot[1], rot[2],
//                scale[0], scale[1], scale[2], 0.7F);
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
        images[0] = R.drawable.dice_1;
        images[1] = R.drawable.dice_2;
        images[2] = R.drawable.dice_3;
        images[3] = R.drawable.dice_4;
        images[4] = R.drawable.dice_5;
        images[5] = R.drawable.dice_6;

//        View mainpagev = getLayoutInflater().inflate(R.layout.mainpage,null,false);
//        ImageView imgview = (ImageView) mainpagev.findViewById(R.id.imageview1);

        com.kt.library.opengl.view.GLRelativeLayout layout = view.findViewById(R.id.mainpage);

        imageview = (ImageView) view.findViewById(R.id.imageview1);

        imageview.setImageResource(images[getDice_num()]);

//        SoundPool soundpool;
//        soundpool = new SoundPool(1, AudioManager.STREAM_MUSIC, 0);
//        int tak = soundpool.load(this, ,1);

        return layout;
    }

    @Override
    public void onHand(int key){
        switch (key) {
            // paper
            case 0b11111:
                break;

            //rock
            case 0b00000:
                ArrayList<String> add = new ArrayList<>();
                ArrayList<String> remove = new ArrayList<>();
                remove.add(ctrId);

                ((MainActivity)MainActivity.mContext).removeandaddViews(remove, add, "gamemap");

                break;
        }
    }

    @Override
    public void onHand(Vector3[] fingersNow){

    }

    @Override
    public void onHand(String gesture){
        switch (gesture){
            case "zoomup":
                ArrayList<String> remove = new ArrayList<>();
                ArrayList<String> add = new ArrayList<>();

                remove.add(ctrId);
                remove.add("gamemap");
                add.add("gamemap_close");
                //add.add("card");

                ((MainActivity)MainActivity.mContext).removeandaddViews(remove, add, "gamemap_close");
        }
    }

    @Override
    public void onDestroyAndRemove(View view, int layoutId) {
    }
}
