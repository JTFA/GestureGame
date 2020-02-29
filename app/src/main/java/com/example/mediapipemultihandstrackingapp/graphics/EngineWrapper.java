package com.example.mediapipemultihandstrackingapp.graphics;

import android.app.Activity;
import android.view.Surface;
import android.widget.RelativeLayout;

import com.example.mediapipemultihandstrackingapp.R;
import com.kt.util.LibraryHelper;
import com.maxst.ar.MaxstAR;
import com.maxst.ar.TrackerManager;
import com.maxst.mkit.MKitJni;
import com.maxst.mkit.MKitJniHelper;

import java.io.File;
import java.util.Iterator;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

public class EngineWrapper {
    private static EngineWrapper instance = null;
    static {
        instance = new EngineWrapper();
    }

    protected boolean isSurfaceChange = false;
    private Queue<Runnable> rendererEvents = new ConcurrentLinkedQueue<>();

    public static EngineWrapper getInstance() {
        return instance;
    }

    private EngineWrapper() {

    }

    public void onCreate(Activity activity) {

        File file = activity.getExternalFilesDir(null);
        String externalPath = file.getPath();

        if (!externalPath.endsWith("/")) {
            externalPath += "/";
        }
        MKitJni.initParam(activity, activity.getAssets(), MKitJniHelper.getInstance(), externalPath); // 게임엔진에서 사용되는 리소스 폴더 세팅


        MKitJni.createGame(); // 게임엔진 렌더링을 위한 게임(MKitRendererManager 클래스) 생성.(3D Scene 개념)
    }


    public void onStart() {

    }


    public void onResume() {
        MaxstAR.onResume(); // Maxst AR SDK에 라이프사이클 상태를 알려줌.
    }


    public void onPause() {
        MaxstAR.onPause(); // Maxst AR SDK에 라이프사이클 상태를 알려줌.
    }


    public void onStop() {

    }


    public void onDestroy() {
        TrackerManager.getInstance().destroyTracker(); // Maxst AR SDK 트래커 해체.
        MaxstAR.deinit(); // Maxst AR SDK 해체.
        MKitJni.destroyGame(); // 게임엔진 해체.
    }


    public void onSurfaceCreate(Surface surface) {
        MKitJni.surfaceCreated(); // 게임엔진에 서피스가 생성 됐음을 알려줌.
        isSurfaceChange = false;
    }


    public void onSurfaceChange(int w, int h) {
        if (!isSurfaceChange) {
            MaxstAR.onSurfaceChanged(w, h); // Maxst AR SDK에 서피스의 사이즈가 변경될때마다 알려줌.
            MKitJni.surfaceChanged(w, h); // 게임엔진에 서피스의 사이즈를 알려줌.
            isSurfaceChange = true;
        }

    }

    public void onSurfaceDestroy() {

    }


    public void processFrame() {
        MKitJni.renderFrame();
        if(MKitJni.isInitialize()) {
            Iterator<Runnable> iterator = rendererEvents.iterator();
            while (iterator.hasNext()) {
                Runnable runnable = iterator.next();
                runnable.run();
                iterator.remove();
            }
        }
    }

    public void setProjectionMatrix(float [] matrix) {
        MKitJni.setProjectionMatrix(matrix);
    }

    public void capture() {
        MKitJniHelper.getInstance().captureScene(true);
    }

    public void queueEvent(Runnable event) {
        rendererEvents.add(event);
    }
}
