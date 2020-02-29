package com.example.mediapipemultihandstrackingapp;

import java.lang.Math;
import java.util.Vector;


public class Vector3 {
    private double x;
    private double y;
    private double z;
    public Vector3(){
        this.x=this.y=this.z=0;
    }
    public Vector3(double x, double y, double z){this.x = x;this.y = y; this.z = z;}

    public double getX(){return this.x;}
    public double getY(){return this.y;}
    public double getZ(){return this.z;}

    public void setX(double input){this.x = input;}
    public void setY(double input){this.y = input;}
    public void setZ(double input){this.z = input;}
    @Override
    public String toString(){return "X: " + x +", "+"Y: " + y +", "+"Z: " + z;}

    public static boolean isZeroVec(Vector3 vec){
        return (getSize(vec)==0)?true:false;
    }

    public static double getSize(Vector3 vec){
        return Math.sqrt((vec.x*vec.x)+(vec.y*vec.y)+(vec.z*vec.z));
    }

    public static double innerProd(Vector3 a, Vector3 b){
        return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
    }

    public static double cosBetween(Vector3 vecA, Vector3 vecB){
        if(isZeroVec(vecA) || isZeroVec(vecB))return (double)2;//which is important in cosine values
        return innerProd(vecA, vecB) / (getSize(vecA) * getSize(vecB));
    }

    public static double radBetween(Vector3 vecA, Vector3 vecB) {
        return Math.acos(cosBetween(vecA, vecB));
    }

    public static double degBetween(Vector3 vecA, Vector3 vecB){
        return radBetween(vecA, vecB)*180/Math.PI;
    }

    public static Vector3 add(Vector3 vecA, Vector3 vecB){
        Vector3 temp = new Vector3();
        temp.setX(vecA.getX()+vecB.getX());
        temp.setY(vecA.getY()+vecB.getY());
        temp.setZ(vecA.getZ()+vecB.getZ());
        return temp;
    }
    public static Vector3 minus(Vector3 vecA, Vector3 vecB){
        Vector3 temp = new Vector3();
        temp.setX(vecA.getX()-vecB.getX());
        temp.setY(vecA.getY()-vecB.getY());
        temp.setZ(vecA.getZ()-vecB.getZ());
        return temp;
    }
    public static void copy(Vector3 vecA, Vector3 vecB){
        vecA.setX(vecB.getX());
        vecA.setY(vecB.getY());
        vecA.setZ(vecB.getZ());
    }

}

