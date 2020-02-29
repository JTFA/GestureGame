package com.example.mediapipemultihandstrackingapp;

import android.util.Log;
import android.view.View;
import android.widget.ImageView;

import com.example.mediapipemultihandstrackingapp.graphics.EngineWrapper;

import java.util.Random;
import java.util.Vector;

public class Listener {
    private int finger;
    private myCustomDelegate view;

    public boolean zoomup_holder = false;
    public boolean zoomout_holder = false;
    public boolean flip_holder = false;
    public boolean movemark_holder = false;

    public void setFinger (int finger){
        this.finger = finger;
    }

    public void setView (myCustomDelegate view) { this.view = view; }

    public void ListenerEvent() {

        view.onHand(this.finger);
    }

    public void gestureEvent(int finger) {
        if (zoomup_holder) {
            switch (finger) {
                case 16:
                    view.onHand("zoomup");
                    Log.d("zoomup", "zoomup!!!!!!");
                    this.zoomup_holder = false;

                case 31:
                    this.zoomup_holder = true;

                default:
                    this.zoomup_holder = false;
            }
        } else {
            if (finger == 31) this.zoomup_holder = true;
        }

        if (zoomout_holder) {
            switch (finger) {
                case 31:
                    view.onHand("zoomout");
                    Log.d("zoomout", "zoomout!!!!!!");
                    this.zoomout_holder = false;

                case 16:
                    this.zoomout_holder = true;

                default:
                    this.zoomout_holder = false;
            }
        } else {
            if (finger == 16) this.zoomout_holder = true;
        }
    }

    public void isFlip(float lm4, float lm20){
        boolean up = (lm4<lm20);

        if (flip_holder) {
            if(up) this.flip_holder = true;
            else {
                Log.d("FLIP", "this is flip~~~~~~~");
                view.onHand("flip");
                this.flip_holder = false;
            }
        }

        else {
            if(up) this.flip_holder = true;
        }
    }

    public void isClick(float landmark8X, float landmark8Y){
        if(landmark8X>0.4 && landmark8X<0.6 &&
                landmark8Y>0.4 && landmark8Y<0.6){
            view.onHand("click");
        }
    }
}