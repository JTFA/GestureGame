package com.example.mediapipemultihandstrackingapp;

import androidx.annotation.NonNull;

public class ABset<T,M> {
    private T myA;
    private M myB;
    //    public ABset<T, M>(){myA = new T(); myB = new M();}
    public T getMyA(){return myA;}
    public M getMyB(){return myB;}
    public void setMyA(T a){myA = a;}
    public void setMyB(M b){myB = b;}

    @NonNull
    @Override
    public String toString() {return myA.toString() + " & " +myB.toString();}
}

