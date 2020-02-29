package com.example.mediapipemultihandstrackingapp;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.GridView;
import android.widget.ImageView;

import java.util.ArrayList;
import java.util.zip.Inflater;

public class ImageAdapter extends BaseAdapter {
    private LayoutInflater inflater;
    private ArrayList<Integer> ImageList = new ArrayList<>();
    private Context context;

    public ImageAdapter(ArrayList<Integer> ImageList){
        this.ImageList = ImageList;
    }
    @Override
    public int getCount() {
        return ImageList.size();
    }

    @Override
    public Object getItem(int position) {
        return null;
    }

    @Override
    public long getItemId(int position) {
        return 0;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        ImageView imageView;
        if (convertView == null) {
            // if it's not recycled, initialize some attributes
            imageView = new ImageView(((MainActivity)MainActivity.mContext));
            //imageView.setLayoutParams(new GridView.LayoutParams(100, 240));
            //imageView.setScaleType(ImageView.ScaleType.FIT_XY);
            imageView.setPadding(1, 1, 1, 1);
        } else {
            imageView = (ImageView) convertView;
        }

        imageView.setLayoutParams(new GridView.LayoutParams(GridView.LayoutParams.MATCH_PARENT, GridView.LayoutParams.MATCH_PARENT));
        //imageView.setLayoutParams(new GridView.LayoutParams(135, 240));
        imageView.setImageResource(ImageList.get(position));
        return imageView;
    }
}
