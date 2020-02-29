package com.maxst.mkit;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.text.TextPaint;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;

/**
 * Utility class
 */
public class MKitUtils {

	private static final String TAG = "MKitUtils";

	/**
	 * Save string to png format. The image file is used for augmented object's texture
	 * @param context Context
	 * @param outFileName file output path
	 * @param text Text for image
	 */
	public static void saveTextToImage(Context context, String outFileName, String text) {
		File newFile = new File(outFileName);
		if (newFile.exists()) {
			newFile.delete();
		}

		Rect textRect = new Rect();
		TextPaint textPaint = new TextPaint();
		textPaint.setAntiAlias(true);
		textPaint.setColor(Color.WHITE);
		textPaint.bgColor = Color.TRANSPARENT;
		textPaint.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD));
		textPaint.getTextBounds(text, 0, text.length(), textRect);

		float density = context.getResources().getDisplayMetrics().density;
		float textSize = 50f;
		textPaint.setTextSize(textSize * density);

		float textWidth = textPaint.measureText(text);
		textPaint.getTextBounds(text, 0, text.length(), textRect);
		Bitmap bitmap = Bitmap.createBitmap(Math.round(textWidth), textRect.height(), Bitmap.Config.ARGB_8888);
		Canvas canvas = new Canvas(bitmap);

		float x = 0f;
		float y = Math.abs(textRect.top);
		y += (textRect.height() - y - Math.abs(textRect.bottom)) / 2;

		canvas.save();
		canvas.drawText(text, x, y, textPaint);
		canvas.restore();

		boolean result = saveBitmapToFile(outFileName, bitmap, Bitmap.CompressFormat.PNG);
		bitmap.recycle();
		Log.d(TAG, "Save Text to image file. result : " + result);
	}

	/**
	 * Save string to png format. The image file is used for augmented object's texture
	 * @param context Context
	 * @param outFileName file output path
	 * @param text Text for image
	 * @param bgResource background for image
	 */
	public static void saveTextToImage2(Context context, String outFileName, String text, int bgResource) {
		File newFile = new File(outFileName);
		if (newFile.exists()) {
			newFile.delete();
		}

		Rect textRect = new Rect();
		TextPaint textPaint = new TextPaint();
		textPaint.setAntiAlias(true);
		textPaint.setColor(Color.WHITE);
		textPaint.bgColor = Color.TRANSPARENT;
		textPaint.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD));
		textPaint.getTextBounds(text, 0, text.length(), textRect);

		float density = context.getResources().getDisplayMetrics().density;
		float textSize = 12.0f;
		textPaint.setTextSize(textSize * density);

//		float textWidth = textPaint.measureText(text);
		textPaint.getTextBounds(text, 0, text.length(), textRect);
		//background image
		BitmapFactory.Options opt = new BitmapFactory.Options();
		opt.inPreferredConfig = Bitmap.Config.ARGB_8888;
		Bitmap temp = BitmapFactory.decodeResource(context.getResources(), bgResource, opt);

//		Log.d(TAG, "bgResource : " + bgResource + " : " + temp);
//		Log.d(TAG, "Text size width/height : " + Math.round(textWidth) + " : " + textRect.height());
//		Log.d(TAG, "bgImage size width/height : " + temp.getWidth() + " : " + temp.getHeight()  + " :: " + Math.round(temp.getWidth()));

		int gapWidth = 22, gapHeight = 8;
		int bgWidth = textRect.width() + (gapWidth * 2);
		int bgHeight = textRect.height() + (gapHeight * 2);

//		Bitmap bgBitmap = Bitmap.createScaledBitmap(temp, Math.round(textWidth), textRect.height(), true);
		Bitmap bgBitmap = Bitmap.createScaledBitmap(temp, bgWidth, bgHeight, true);
		Canvas canvas = new Canvas(bgBitmap);


		float x = 0f;
		float y = Math.abs(textRect.top);
		y += (textRect.height() - y - Math.abs(textRect.bottom)) / 2;

		canvas.save();
//		canvas.drawText(text, x, y, textPaint);
		canvas.drawText(text, x + gapWidth, y + gapHeight, textPaint);
		canvas.restore();

		boolean result = saveBitmapToFile(outFileName, bgBitmap, Bitmap.CompressFormat.PNG);
		temp.recycle();
		bgBitmap.recycle();
		Log.d(TAG, "Save Text to image file 2. result : " + result);
	}

	/**
	 * Save bitmap to file
	 * @param fileName output file name
	 * @param bitmap bitmap source
	 * @param format image file format
	 * @return success to save file or not
	 */
	private static boolean saveBitmapToFile(String fileName, Bitmap bitmap, Bitmap.CompressFormat format) {

		FileOutputStream fileOutputStream = null;

		try {
			fileOutputStream = new FileOutputStream(fileName);
			bitmap.compress(format, 100, fileOutputStream);
			fileOutputStream.close();
			return true;
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (fileOutputStream != null) { try { fileOutputStream.close(); } catch (Exception e) { e.printStackTrace(); } }
		}
		return false;
	}
}
