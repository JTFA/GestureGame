package com.example.mediapipemultihandstrackingapp.view;

import android.app.Activity;
import android.util.Log;
import android.view.View;
import android.widget.ListView;
import android.widget.Toast;

import com.example.mediapipemultihandstrackingapp.R;
import com.example.mediapipemultihandstrackingapp.graphics.EngineWrapper;
import com.kt.library.opengl.view.CustomViewDelegate;
import com.kt.library.opengl.view.IGLLayout;
import com.kt.util.LibraryHelper;
import com.maxst.mkit.Listener;
import com.maxst.mkit.MKitJni;
import com.maxst.mkit.MKitJniHelper;
import com.maxst.mkit.MKitUtils;

public class ARTestLayout implements CustomViewDelegate {
    public static String ctrId = "CustomView1";
    public static int layout = R.layout.gamemap;
    public Activity activity;

    public ARTestLayout(Activity activity) {
        this.activity = activity;
    }

    public boolean showMenu() {
        LibraryHelper.addCustomViewDelegate(layout, this);

        EngineWrapper.getInstance().queueEvent(() -> {
            int[] size = { 1080, 1920 };
            float[] pos = {0f, 0f, 5f};
            float[] rot = {0f, 0f, 0f};
            float[] scale = {1f, 1f, 1f};
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
        EngineWrapper.getInstance().queueEvent(()->{
            MKitJniHelper.getInstance().removeCustomViewLayout(ctrId);
        });
    }

    @Override
    public IGLLayout onCreateAndAdd(View view, int layoutId) {
        android.util.Log.d("GameEngineCore", "onCreateAndAdd");
        com.kt.library.opengl.view.GLRelativeLayout layout = view.findViewById(R.id.gamemap);

        final String[] LIST_MENU = {"LIST1", "LIST2", "LIST3", "LIST4", "LIST5",
                "LIST6", "LIST7", "LIST8", "LIST9", "LIST10",
                "LIST11", "LIST12", "LIST13", "LIST14", "LIST15",
                "LIST16", "LIST17", "LIST18", "LIST19", "LIST20",
                "LIST21", "LIST22", "LIST23", "LIST24", "LIST25"} ;
        ListView listView = layout.findViewById(R.id.gamemap);
        android.widget.ArrayAdapter adapter = new android.widget.ArrayAdapter(activity,
                android.R.layout.simple_list_item_1, LIST_MENU) ;
        listView.setAdapter(adapter);
        listView.setOnItemClickListener((parent, v, position, id) -> {
            String strText = (String) parent.getItemAtPosition(position);
            Toast.makeText(activity, strText, Toast.LENGTH_SHORT).show();
        });

        return layout;
    }

    @Override
    public void onDestroyAndRemove(View view, int layoutId) {
    }
}
