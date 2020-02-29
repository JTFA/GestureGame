package com.example.mediapipemultihandstrackingapp;

import com.kt.library.opengl.view.CustomViewDelegate;

public interface myCustomDelegate extends CustomViewDelegate {
    public boolean showMenu();
    public void removeMenu();

    void onHand(int key);
    void onHand(Vector3[] fingersNow);
    void onHand(String gesture);
}
