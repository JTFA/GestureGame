package com.example.mediapipemultihandstrackingapp;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.SurfaceTexture;
import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;

import android.util.Log;
import android.util.Size;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.RelativeLayout;

import com.example.mediapipemultihandstrackingapp.graphics.EngineWrapper;
import com.example.mediapipemultihandstrackingapp.graphics.ExternalTextureConverter2;
import com.example.mediapipemultihandstrackingapp.view.Card;
import com.example.mediapipemultihandstrackingapp.view.End;
import com.example.mediapipemultihandstrackingapp.view.GameMap;
import com.example.mediapipemultihandstrackingapp.view.GameMap_close;
import com.example.mediapipemultihandstrackingapp.view.MainPage;
import com.example.mediapipemultihandstrackingapp.view.Start;
import com.google.mediapipe.components.FrameProcessor;
import com.google.mediapipe.formats.proto.LandmarkProto.NormalizedLandmark;
import com.google.mediapipe.formats.proto.LandmarkProto.NormalizedLandmarkList;
import com.google.mediapipe.components.CameraHelper;
import com.google.mediapipe.components.CameraXPreviewHelper;
import com.google.mediapipe.components.PermissionHelper;
import com.google.mediapipe.framework.AndroidAssetUtil;
import com.google.mediapipe.framework.PacketGetter;
import com.google.mediapipe.glutil.EglManager;
import com.google.protobuf.InvalidProtocolBufferException;
import com.kt.util.LibraryHelper;
import com.maxst.mkit.MKitJni;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

/** Main activity of MediaPipe example apps. */
public class MainActivity extends AppCompatActivity {
    public static Context mContext;

    private static final String TAG = "MainActivity";
    private static final String BINARY_GRAPH_NAME = "handtrackinggpu.binarypb";
    private static final String INPUT_VIDEO_STREAM_NAME = "input_video";
    private static final String OUTPUT_VIDEO_STREAM_NAME = "output_video";
    private static final String OUTPUT_HAND_PRESENCE_STREAM_NAME = "hand_presence";
    private static final String OUTPUT_LANDMARKS_STREAM_NAME = "hand_landmarks";
    private static final CameraHelper.CameraFacing CAMERA_FACING = CameraHelper.CameraFacing.BACK;

    // Flips the camera-preview frames vertically before sending them into FrameProcessor to be
    // processed in a MediaPipe graph, and flips the processed frames back when they are displayed.
    // This is needed because OpenGL represents images assuming the image origin is at the bottom-left
    // corner, whereas MediaPipe in general assumes the image origin is at top-left.
    private static final boolean FLIP_FRAMES_VERTICALLY = true;
    static {
        // Load all native libraries needed by the app.
        System.loadLibrary("mediapipe_jni");
        System.loadLibrary("opencv_java3");
    }
    // {@link SurfaceTexture} where the camera-preview frames can be accessed.
    private SurfaceTexture previewFrameTexture;

    private static final int THUMB_ANGLE = 17;
    private static final int FINGER_ANGLE =75;
    private static final int FRAME_WINDOW_SIZE = 5;
    private static final int FRAME_ACCEPTABLE = 2;

    // {@link SurfaceView} that displays the camera-preview frames processed by a MediaPipe graph.
    private SurfaceView previewDisplayView;
    // Creates and manages an {@link EGLContext}.
    private EglManager eglManager;
    // Sends camera-preview frames into a MediaPipe graph for processing, and displays the processed
    // frames onto a {@link Surface}.
    private FrameProcessor processor;
    // Converts the GL_TEXTURE_EXTERNAL_OES texture from Android camera into a regular texture to be
    // consumed by {@link FrameProcessor} and the underlying MediaPipe graph.
    private ExternalTextureConverter2 converter;
    // Handles camera access via the {@link CameraX} Jetpack support library.
    private CameraXPreviewHelper cameraHelper;
    private Map<String, myCustomDelegate> layouts;
    private Start start;
    private MainPage mainpage;
    private GameMap gamemap;
    private GameMap_close gamemap_close;
    private End end;
    private Card card;
    private int count =0;

    private int lastgesture;
    Listener gestureListener = new Listener();
    private boolean flip_holder = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        previewDisplayView = new SurfaceView(this);
        setupPreviewDisplayView();
        mContext = this;
        // Initialize asset manager so that MediaPipe native libraries can access the app assets, e.g.,
        // binary graphs.

        int[] frame = {0,0,0,0,0};
        layouts = new HashMap<String, myCustomDelegate>();

        AndroidAssetUtil.initializeNativeAssetManager(this);
        eglManager = new EglManager(null);
        processor =
                new FrameProcessor(
                        this,
                        eglManager.getNativeContext(),
                        BINARY_GRAPH_NAME,
                        INPUT_VIDEO_STREAM_NAME,
                        OUTPUT_VIDEO_STREAM_NAME);

        processor.getVideoSurfaceOutput().setFlipY(FLIP_FRAMES_VERTICALLY);
        processor.addPacketCallback(
                OUTPUT_HAND_PRESENCE_STREAM_NAME,
                (packet) -> {

                    Boolean handPresence = PacketGetter.getBool(packet);
                    if (!handPresence) {
                        Log.d(
                                TAG,
                                "[TS:" + packet.getTimestamp() + "] Hand presence is false, no hands detected.");
                    }
                });


        processor.addPacketCallback(
                OUTPUT_LANDMARKS_STREAM_NAME,
                (packet) -> {
                    ArrayList<ABset<Boolean, Vector3>> fingerState = new ArrayList<>();
                    Vector3[] temp;

                    byte[] landmarksRaw = PacketGetter.getProtoBytes(packet);
                    try {
                        NormalizedLandmarkList landmarks = NormalizedLandmarkList.parseFrom(landmarksRaw);
                        if (landmarks == null) {
                            Log.d(TAG, "[TS:" + packet.getTimestamp() + "] No hand landmarks.");
                            return;
                        }

                        if(landmarks != null){
                            fingerState=getFingerState(landmarks,frame);
                            if(fingerState != null){
                                temp = finger(fingerState);
                                String tempS = Integer.toBinaryString((int) temp[0].getX()) + "   " +
                                        temp[1] + " " +
                                        temp[2] + " " +
                                        temp[3] + " " +
                                        temp[4] + " " +
                                        temp[5] + "   " +
                                        Integer.toString(frame[0]) + " " +
                                        Integer.toString(frame[1]) + " " +
                                        Integer.toString(frame[2]) + " " +
                                        Integer.toString(frame[3]) + " " +
                                        Integer.toString(frame[4]);


                                lastgesture = (int) temp[0].getX();

                                ////////////////////////////////////////////////////////////////////////////////////
                                float landmark20 = landmarks.getLandmarkList().get(20).getX();
                                float landmark4 = landmarks.getLandmarkList().get(4).getX();

                                float landmark8X = landmarks.getLandmarkList().get(8).getX();
                                float landmark8Y = landmarks.getLandmarkList().get(8).getY();

                                if(lastgesture==0b01000){
                                    //flip_holder = true;
                                    gestureListener.isClick(landmark8X, landmark8Y);
                                }

                                gestureListener.setFinger(lastgesture);
                                gestureListener.ListenerEvent();

                                gestureListener.gestureEvent(lastgesture);
                                //gestureListener.movemarker(lastgesture, landmark4, landmark4y);

                                //decide whether flip or not only if paper
//                                if(flip_holder) {
//                                    if(lastgesture == 0b01000) flip_holder = true;
//                                    else if(lastgesture == 31) {
//                                        //count++;
//                                        gestureListener.isFlip(landmark4, landmark20);
//                                    }
//                                    else flip_holder = false;
//                                }

                                if(lastgesture == 31) {
                                    //count++;
                                    //gestureListener.isFlip(landmark4, landmark20);
                                }

                                Log.d("FINGER","fing==> "+stringFinger(lastgesture));
                                Log.d("FINGER","fing==> "+lastgesture);

                                //Log.d("landmark===>",": "+getLandmarksDebugString(landmarks));

                            }


                        }else {
                        }

                    }catch(InvalidProtocolBufferException e){
                        Log.e(TAG, "Couldn't Exception received -"+e);
                        return;
                    }

                });

        PermissionHelper.checkAndRequestCameraPermissions(this);
        EngineWrapper.getInstance().onCreate(this);

        //확장 랜더링 라이브러리 레이아웃 설정
        RelativeLayout webRender = findViewById(R.id.web_render_layout);
        LibraryHelper.initExternalRender(webRender);


        //arTestLayout = new ARTestLayout(this, "artestlayout");
        start = new Start(this, "start");
        mainpage = new MainPage(this, "mainpage");
        gamemap = new GameMap(this, "gamemap");
        gamemap_close = new GameMap_close(this, "gamemap_close");
        end = new End(this, "end");
        //card = new Card(this, "card");

        layouts.put("start", start);
        layouts.put("mainpage", mainpage);
        layouts.put("gamemap_close", gamemap_close);
        layouts.put("gamemap", gamemap);
        layouts.put("end", end);
        //layouts.put("card", card);

        previewDisplayView.postDelayed(() -> {
            layouts.get("start").showMenu();
            gestureListener.setView(layouts.get("start"));
        }, 2000);
    }

//    public void changeMenu(String here, String there){
//        if(here==null){
//            layouts.get("start").showMenu();
//            gestureListener.setView(layouts.get("start"));
//        }
//        else{
//            layouts.get(here).removeMenu();
//            layouts.get(there).showMenu();
//            gestureListener.setView(layouts.get(there));
//        }
//    }

    public void removeandaddViews(ArrayList<String> remove, ArrayList<String> add, String listenerview){
        int removenum = remove.size();
        int addnum = add.size();

        if(!remove.isEmpty()){
            for(int i=0;i<removenum;i++){
                Log.d("KKANG", "remove"+remove.get(i));
                layouts.get(remove.get(i)).removeMenu();
            }
        }

        for(int i=0;i<addnum;i++){
            Log.d("KKANG", "add"+add.get(i));
            layouts.get(add.get(i)).showMenu();
        }

        gestureListener.setView(layouts.get(listenerview));
    }

    @Override
    protected void onResume() {
        super.onResume();
        converter = new ExternalTextureConverter2(eglManager.getContext());
        converter.setFlipY(FLIP_FRAMES_VERTICALLY);
        converter.setConsumer(processor);

        if (PermissionHelper.cameraPermissionsGranted(this)) {
            startCamera();
        }
        EngineWrapper.getInstance().onResume();
    }
    @Override
    protected void onPause() {
        super.onPause();
        converter.close();
        EngineWrapper.getInstance().onPause();
    }

    @Override
    public void onRequestPermissionsResult(
            int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        PermissionHelper.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }

    private void setupPreviewDisplayView() {
        previewDisplayView.setVisibility(View.GONE);
        ViewGroup viewGroup = findViewById(R.id.preview_display_layout);
        viewGroup.addView(previewDisplayView);
        previewDisplayView.setOnTouchListener(touchListener);

        previewDisplayView
                .getHolder()
                .addCallback(
                        new SurfaceHolder.Callback() {
                            @Override
                            public void surfaceCreated(SurfaceHolder holder) {
                                processor.getVideoSurfaceOutput().setSurface(holder.getSurface());
                                EngineWrapper.getInstance().onSurfaceCreate(holder.getSurface());
                            }
                            @Override
                            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
                                // (Re-)Compute the ideal size of the camera-preview display (the area that the
                                // camera-preview frames get rendered onto, potentially with scaling and rotation)
                                // based on the size of the SurfaceView that contains the display.
                                Size viewSize = new Size(width, height);
                                Size displaySize = cameraHelper.computeDisplaySizeFromViewSize(viewSize);
                                // Connect the converter to the camera-preview frames as its input (via
                                // previewFrameTexture), and configure the output width and height as the computed
                                // display size.
                                converter.setSurfaceTextureAndAttachToGLContext(
                                        previewFrameTexture, displaySize.getWidth(), displaySize.getHeight());
                                EngineWrapper.getInstance().onSurfaceChange(width, height);
                            }
                            @Override
                            public void surfaceDestroyed(SurfaceHolder holder) {
                                processor.getVideoSurfaceOutput().setSurface(null);
                                EngineWrapper.getInstance().onSurfaceDestroy();
                            }
                        });
    }
    private void startCamera() {
        cameraHelper = new CameraXPreviewHelper();
        cameraHelper.setOnCameraStartedListener(
                surfaceTexture -> {
                    previewFrameTexture = surfaceTexture;
                    // Make the display view visible to start showing the preview. This triggers the
                    // SurfaceHolder.Callback added to (the holder of) previewDisplayView.
                    previewDisplayView.setVisibility(View.VISIBLE);
                });
        cameraHelper.startCamera(this, CAMERA_FACING, /*surfaceTexture=*/ null);
    }

    private static String getLandmarksDebugString(NormalizedLandmarkList landmarks) {
        int landmarkIndex = 0;
        String landmarksString = "";
        for (NormalizedLandmark landmark : landmarks.getLandmarkList()) {
            if(landmarkIndex==4){
                landmarksString +=
                        "\t\tLandmark["
                                + landmarkIndex
                                + "]: ("
                        + landmark.getX()
                        + ", "
//                        + landmark.getY()
//                        + ", "
//                                + landmark.getZ()
                                + ")\n";
            }

            ++landmarkIndex;
        }
        return landmarksString;
    }


    public ArrayList<ABset<Boolean, Vector3>> getFingerState(NormalizedLandmarkList HandLandmarks, int[] frame){
        ArrayList<ABset<Boolean, Vector3>> result = new ArrayList<>();
        ArrayList<Vector3> firstHand = new ArrayList<>();
        Vector3 tempVec;

        ABset<Boolean,Vector3> tempSet;
        Vector3 firstJoint,secondJoint;
        double angle;

        int handIndex = 0;
        int landmarkIndex = 0;
        for (NormalizedLandmark landmark : HandLandmarks.getLandmarkList()) {
            //////////build vector3////////////
            if(handIndex == 0){
                tempVec = new Vector3(landmark.getX(),landmark.getY(),0);
                firstHand.add(tempVec);
            }
            ///////////////////////////////////
            ++landmarkIndex;
        }

        //for the thumb
        firstJoint = Vector3.minus(firstHand.get(3),firstHand.get(2));
        secondJoint = Vector3.minus(firstHand.get(4),firstHand.get(3));
        tempSet = new ABset<>();
        angle = Vector3.degBetween(firstJoint, secondJoint);
        //false : folded             true : stretched
        int frameChk = (angle<THUMB_ANGLE) ?1:-1;
        if(frame[0]>=0 && frame[0]<=FRAME_WINDOW_SIZE){
            frame[0]+=frameChk;
        }
        if(frame[0]<0)frame[0]=0;
        if(frame[0]>FRAME_WINDOW_SIZE)frame[0]=FRAME_WINDOW_SIZE;
        boolean bool = (frame[0]>=FRAME_WINDOW_SIZE-FRAME_ACCEPTABLE) ?true:false;
        tempSet.setMyA(bool);
        tempSet.setMyB(firstHand.get(4));
        result.add(tempSet);

        //except the thumb
        for(int i=1;i<5;i++){
            firstJoint = Vector3.minus(firstHand.get(4*i+2),firstHand.get(4*i+1));
            secondJoint = Vector3.minus(firstHand.get(4*i+3),firstHand.get(4*i+2));
            tempSet = new ABset<>();
            angle = Vector3.degBetween(firstJoint, secondJoint);
            //false : folded             true : stretched
            ///ex) size =5 acceptable 2
            //frame can be 0  1  2 || 3  4  5
            //          false here || true here
            frameChk = (angle<FINGER_ANGLE) ?1:-1;
            if(frame[i]>=0 && frame[i]<=FRAME_WINDOW_SIZE){
                frame[i]+=frameChk;
            }
            if(frame[i]<0)frame[i]=0;
            if(frame[i]>FRAME_WINDOW_SIZE)frame[i]=FRAME_WINDOW_SIZE;
            bool = (frame[i]>=FRAME_WINDOW_SIZE-FRAME_ACCEPTABLE) ?true:false;

            tempSet.setMyA(bool);
            tempSet.setMyB(firstHand.get(4*i+4));

            result.add(tempSet);
        }
        return result;


    }

    public Vector3[] finger(ArrayList<ABset<Boolean, Vector3>> fingerState){
        Vector3[] resultVector = new Vector3[6];
        for(int i=0;i<6;i++)resultVector[i] = new Vector3();
        int[] stretch = {16,8,4,2,1};//Thumb, Index, Middle, Ring, Little
        int sum=0;

        String resultString="Now : ";
        String[] fingers={" 1 "," 2 "," 3 " ," 4 "," 5 "};

        int i=0;
        for(ABset<Boolean, Vector3> abset : fingerState){
            if(abset.getMyA()){
                resultString+=fingers[i];
                sum+=stretch[i];
            }
            else {
                resultString+="___";

            }
            i++;
        }
        resultVector[0].setX(sum);
        for(i=0;i<5;i++){
            Vector3.copy(resultVector[i+1],fingerState.get(i).getMyB());
        }

        return resultVector;
    }


    public String stringFinger(int finger){
        String result="THIS IS ";
        int sum=0;
        int temp=finger;
        for(int i=0;i<5;i++){
            if(temp%2==1)sum++;
            temp=temp>>1;
        }
        Log.d("Tag", finger+"===> "+temp);
        switch(finger){
            case 0b01000 : {
                result+="Click";
                break;
            }
            case 0b00000 : {
                result+="Rock";
                break;
            }
            case 0b11111 : {
                result+="Paper";
                break;
            }
            case 0b01100 : {
                result="YOU";
                break;
            }
            case 0b10001 :{
                result+="Call";
                break;
            }
            case 0b11001 :{
                result+="SpiderMan";
                break;
            }
            case 0b11000:{
                result+="Scissors";
                break;
            }

            default:{
                switch(sum){
                    case 0 :{
                        result = "THIS IS 0";
                        break;
                    }
                    case 1:{
                        result = "THIS ALSO 1";
                        break;
                    }
                    case 2:{
                        result = "THIS ALSO 2";
                        break;
                    }
                    case 3:{
                        result = "THIS ALSO 3";
                        break;
                    }
                    case 4:{
                        result = "THIS ALSO 4";
                        break;
                    }
                }//end of default switch
            }//end of default
        }//end of switch
        return result;
    }

    @SuppressLint("ClickableViewAccessibility")
    private View.OnTouchListener touchListener = (v, event) -> {
        switch (event.getAction()) {
            //처음 눌렸을때
            case MotionEvent.ACTION_DOWN:
                Log.d("TAG", "(ONTOUCHx===>"+event.getX());
                Log.d("TAG", "ONTOUCHy)===>"+event.getY());
                MKitJni.touchDown(event.getAction(), event.getX(), event.getY(), event.getPointerId(0));
                break;
            //떼었을 때
            case MotionEvent.ACTION_UP:
                MKitJni.touchUp(event.getAction(), event.getX(), event.getY(), event.getPointerId(0));
                break;
            //움직였을 때
            case MotionEvent.ACTION_MOVE:
                MKitJni.touchMove(event.getAction(), event.getX(), event.getY(), event.getPointerId(0));
                break;
        }
        return true;
    };
}
