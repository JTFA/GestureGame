package com.kt.util;

import android.view.View;
import android.widget.RelativeLayout;

import com.kt.library.opengl.view.CustomViewDelegate;
import com.kt.library.opengl.view.IGLLayout;
import com.kt.library.opengl.view.ViewGroupManager;

import java.util.HashMap;
import java.util.Map;

public class LibraryHelper {

    private static Map<Integer, CustomViewDelegate> viewDelegates = new HashMap<>();

    private static CustomViewDelegate innerDelegate = new CustomViewDelegate() {
        @Override
        public IGLLayout onCreateAndAdd(View view, int layoutId) {
            CustomViewDelegate delegate = viewDelegates.get(layoutId);
            if (delegate != null) {
                return delegate.onCreateAndAdd(view, layoutId);
            }
            return null;
        }

        @Override
        public void onDestroyAndRemove(View view, int layoutId) {
            CustomViewDelegate delegate = viewDelegates.get(layoutId);
            if (delegate != null) {
                delegate.onDestroyAndRemove(view, layoutId);
                viewDelegates.remove(delegate);
            }
        }
    };

    public static void initExternalRender(RelativeLayout layout) {
        ViewGroupManager.getInstance().initialize(layout);
        ViewGroupManager.getInstance().setCustomViewDelegate(innerDelegate);
    }

    public static void addCustomViewDelegate(int layoutId, CustomViewDelegate delegate) {
        viewDelegates.put(layoutId, delegate);
    }
}
