/*
 * Copyright (c) 2013 Chun-Ying Huang
 *
 * This file is part of GamingAnywhere (GA).
 *
 * GA is free software; you can redistribute it and/or modify it
 * under the terms of the 3-clause BSD License as published by the
 * Free Software Foundation: http://directory.fsf.org/wiki/License:BSD_3Clause
 *
 * GA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the 3-clause BSD License along with GA;
 * if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

package org.gaminganywhere.gaclient.util;

import org.gaminganywhere.gaclient.util.Pad.PartitionEventListener;

import android.content.Context;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class GAControllerNDS extends GAController implements
	OnClickListener, PartitionEventListener
{
	private Button buttonL = null;
	private Button buttonR = null;
	private Button buttonToggleLPad = null;
	private Button buttonToggleRPad = null;
	private Button buttonToggleLRBtn = null;
	private Button buttonEsc = null;
	private Button buttonBack = null;
	private Button buttonSelect = null;
	private Button buttonStart = null;
	private Pad padLeft = null;	
//	private Pad padRight = null;
	private Button buttonA = null;
	private Button buttonB = null;
	private Button buttonX = null;
	private Button buttonY = null;

	public GAControllerNDS(Context context) {
		super(context);
	}

	public static String getName() {
		return "NDS";
	}
	
	public static String getDescription() {
		return "Emulated NDS controller";
	}

	int currWidth = -1;
	int currHeight = -1;
	boolean showLPad = true;
	boolean showRPad = true;
	boolean showLRBtn = true;
	@Override
	public void onDimensionChange(int width, int height) {
		// assume Portrait layout
		int keyBtnWidth = width/6;	// top has 5 keys, leave one for spacing
		int keyBtnHeight = height/16;
		int padSize = height/4;
		int margin = keyBtnWidth/6;
		// check width/height
		if(width < 0 || height < 0)
			return;
		// must be called first
		super.setMouseVisibility(false);
		super.onDimensionChange(width, height);
		currWidth = width;
		currHeight = height;
		//
		buttonToggleLPad = newButton("LPad", margin, margin, keyBtnWidth, keyBtnHeight);
		buttonToggleLPad.setOnClickListener(this);
		//
		buttonToggleLRBtn = newButton("L/R Button", margin+(margin+keyBtnWidth)*1, margin, keyBtnWidth, keyBtnHeight);
		buttonToggleLRBtn.setOnClickListener(this);
		//
		buttonBack = newButton("<<", margin+(margin+keyBtnWidth)*2, margin, keyBtnWidth, keyBtnHeight);
		buttonBack.setOnClickListener(this);
		//
		buttonEsc = newButton("ESC", margin+(margin+keyBtnWidth)*3, margin, keyBtnWidth, keyBtnHeight);
		buttonEsc.setOnTouchListener(this);
		//
		buttonToggleRPad = newButton("RPad", margin+(margin+keyBtnWidth)*4, margin, keyBtnWidth, keyBtnHeight);
		buttonToggleRPad.setOnClickListener(this);
		//
		buttonSelect = newButton("Select",
				width-margin-keyBtnWidth, height-margin-keyBtnHeight,
				keyBtnWidth, keyBtnHeight);
		buttonSelect.setOnTouchListener(this);
		//
		buttonStart = newButton("Start",
				width-margin-keyBtnWidth, height-margin*2-keyBtnHeight*2,
				keyBtnWidth, keyBtnHeight);
		buttonStart.setOnTouchListener(this);
		//
		if(showLRBtn) {
			buttonL = newButton("L", margin, height-margin*2-padSize-keyBtnHeight, padSize, keyBtnHeight);
			buttonL.setOnTouchListener(this);
			//
			buttonR = newButton("R", width-margin-padSize, height-margin-padSize, padSize, keyBtnHeight);
			buttonR.setOnTouchListener(this);
		} else {
			buttonL = null;
			buttonR = null;
		}
		//
		if(showLPad) {
			padLeft = new Pad(getContext());
			padLeft.setAlpha((float) 0.5);
			padLeft.setOnTouchListener(this);
			padLeft.setPartition(12);
			padLeft.setPartitionEventListener(this);
			padLeft.setDrawPartitionAll(false);
			placeView(padLeft, margin, height-margin-padSize, padSize, padSize);
		} else {
			padLeft = null;
		}
		//
		if(showRPad) {
			// Pad-based ABXY
//			padRight = null;
//			padRight = new Pad(getContext());
//			padRight.setAlpha((float) 0.5);
//			padRight.setOnTouchListener(this);
//			padRight.setPartition(8);
//			padRight.setPartitionEventListener(this);
//			padRight.setDrawPartitionAll(false);
//			padRight.setDrawPartitionLine(new int[] {1, 3, 5, 7});
//			padRight.setDrawLabel(new int[] {3, 5, 1, 3, 5, 7, 7, 1}, new String[] {"B", "A", "Y", "X"});
//			placeView(padRight,
//					width-margin-padSize, height-margin*2-padSize*2, padSize, padSize);
			int cx, cy, gap, bsize;
			cx = width-margin-padSize/2;
			cy = height-margin*2-padSize-padSize/2;
			gap = (int) (padSize*0.3/2);
			bsize = (int) (padSize*0.7/2);
			buttonA = newButton("A", cx+gap, cy-bsize/2, bsize, bsize);
			buttonA.setOnTouchListener(this);
			//
			buttonB = newButton("B", cx-bsize/2, cy+gap, bsize, bsize);
			buttonB.setOnTouchListener(this);
			//
			buttonX = newButton("X", cx-bsize/2, cy-gap-bsize, bsize, bsize);
			buttonX.setOnTouchListener(this);
			//
			buttonY = newButton("Y", cx-gap-bsize, cy-bsize/2, bsize, bsize);
			buttonY.setOnTouchListener(this);
		} else {
//			padRight = null;
		}
	}
	
	private float lastX = -1;
	private float lastY = -1;
	private int lastButton = -1;
	@Override
	public boolean onTouch(View v, MotionEvent evt) {
		int count = evt.getPointerCount();
		int action = evt.getActionMasked();
		float x = evt.getX();
		float y = evt.getY();
		//
		if(count==1 && (v == padLeft/* || v == padRight*/)) {
			if(((Pad) v).onTouch(evt));
				return true;
		}
		if(v == buttonA)
			return handleButtonTouch(action, SDL2.Scancode.X, SDL2.Keycode.x, 0, 0);
		if(v == buttonB)
			return handleButtonTouch(action, SDL2.Scancode.Z, SDL2.Keycode.z, 0, 0);
		if(v == buttonX)
			return handleButtonTouch(action, SDL2.Scancode.S, SDL2.Keycode.s, 0, 0);
		if(v == buttonY)
			return handleButtonTouch(action, SDL2.Scancode.A, SDL2.Keycode.a, 0, 0);
		if(v == buttonL)
			return handleButtonTouch(action, SDL2.Scancode.Q, SDL2.Keycode.q, 0, 0);
		if(v == buttonR)
			return handleButtonTouch(action, SDL2.Scancode.W, SDL2.Keycode.w, 0, 0);
		if(v == buttonEsc)
			return handleButtonTouch(action, SDL2.Scancode.ESCAPE, SDL2.Keycode.ESCAPE, 0, 0);
		if(v == buttonSelect)
			return handleButtonTouch(action, SDL2.Scancode.RSHIFT, SDL2.Keycode.RSHIFT, 0/*SDL2.Keymod.RSHIFT*/, 0);
		if(v == buttonStart)
			return handleButtonTouch(action, SDL2.Scancode.RETURN, SDL2.Keycode.RETURN, 0, 0);
		// must be called last
		// XXX: not calling super.onTouch() because we have our own handler
		//return super.onTouch(v, evt);
		if(v == this.getPanel()) {
			switch(action) {
			case MotionEvent.ACTION_DOWN:
			//case MotionEvent.ACTION_POINTER_DOWN:
				if(count==1) {
					lastX = x;
					lastY = y;
					lastButton = SDL2.Button.LEFT;
					sendMouseKey(true, SDL2.Button.LEFT, x, y);
				}
				break;
			case MotionEvent.ACTION_UP:
			//case MotionEvent.ACTION_POINTER_UP:
				if(count == 1 && lastButton != -1) {
					sendMouseKey(false, SDL2.Button.LEFT, x, y);
					lastButton = -1;
				}
				break;
			case MotionEvent.ACTION_MOVE:
				if(count == 1) {
					float dx = x-lastX;
					float dy = y-lastY;
					sendMouseMotion(x, y, dx, dy, 0, /*relative=*/false);
					lastX = x;
					lastY = y;
				}
				break;
			}
			return true;
		}
		return false;
	}

	private boolean keyLeft = false;
	private boolean keyRight = false;
	private boolean keyUp = false;
	private boolean keyDown = false;
	private void emulateArrowKeys(int action, int part) {
		boolean myKeyLeft, myKeyRight, myKeyUp, myKeyDown;
		myKeyLeft = keyLeft;
		myKeyRight = keyRight;
		myKeyUp = keyUp;
		myKeyDown = keyDown;
		switch(action) {
		case MotionEvent.ACTION_DOWN:
		case MotionEvent.ACTION_POINTER_DOWN:
		case MotionEvent.ACTION_MOVE:
			// partition mappings for keys:
			// - up: 11, 12, 1, 2
			// - right: 2, 3, 4, 5
			// - down: 5, 6, 7, 8
			// - left: 8, 9, 10, 11
			switch(part) {
			case 0:
				myKeyUp = myKeyRight = myKeyDown = myKeyLeft = false;
				break;
			// single keys
			case 12: case 1:
				myKeyUp = true;
				myKeyRight = myKeyDown = myKeyLeft = false;
				break;
			case 3: case 4:
				myKeyRight = true;
				myKeyUp = myKeyDown = myKeyLeft = false;
				break;
			case 6: case 7:
				myKeyDown = true;
				myKeyUp = myKeyRight = myKeyLeft = false;
				break;
			case 9: case 10:
				myKeyLeft = true;
				myKeyUp = myKeyRight = myKeyDown = false;
				break;
			// hybrid keys
			case 2:
				myKeyUp = myKeyRight = true;
				myKeyDown = myKeyLeft = false;
				break;
			case 5:
				myKeyRight = myKeyDown = true;
				myKeyUp = myKeyLeft = false;
				break;
			case 8:
				myKeyDown = myKeyLeft = true;
				myKeyUp = myKeyRight = false;
				break;
			case 11:
				myKeyLeft = myKeyUp = true;
				myKeyRight = myKeyDown = false;
				break;
			}
			break;
		case MotionEvent.ACTION_UP:
		case MotionEvent.ACTION_POINTER_UP:
			if(keyLeft)
				this.sendKeyEvent(false, SDL2.Scancode.LEFT, 0, 0, 0);
			if(keyRight)
				this.sendKeyEvent(false, SDL2.Scancode.RIGHT, 0, 0, 0);
			if(keyUp)
				this.sendKeyEvent(false, SDL2.Scancode.UP, 0, 0, 0);
			if(keyDown)
				this.sendKeyEvent(false, SDL2.Scancode.DOWN, 0, 0, 0);
			myKeyUp = myKeyRight = myKeyDown = myKeyLeft = false;
			break;
		}
		if(myKeyUp != keyUp) {
			this.sendKeyEvent(myKeyUp, SDL2.Scancode.UP, SDL2.Keycode.UP, 0, 0);
			//Log.d("ga_log", String.format("Key up %s", myKeyUp ? "down" : "up"));
		}
		if(myKeyDown != keyDown) {
			this.sendKeyEvent(myKeyDown, SDL2.Scancode.DOWN, SDL2.Keycode.DOWN, 0, 0);
			//Log.d("ga_log", String.format("Key down %s", myKeyDown ? "down" : "up"));
		}
		if(myKeyLeft != keyLeft) {
			this.sendKeyEvent(myKeyLeft, SDL2.Scancode.LEFT, SDL2.Keycode.LEFT, 0, 0);
			//Log.d("ga_log", String.format("Key left %s", myKeyLeft ? "down" : "up"));
		}
		if(myKeyRight != keyRight) {
			this.sendKeyEvent(myKeyRight, SDL2.Scancode.RIGHT, SDL2.Keycode.RIGHT, 0, 0);
			//Log.d("ga_log", String.format("Key right %s", myKeyRight ? "down" : "up"));
		}
		keyUp = myKeyUp;
		keyDown = myKeyDown;
		keyLeft = myKeyLeft;
		keyRight = myKeyRight;
	}
	
//	private int lastScan = -1;
//	private int lastKey = -1;
//	private void emulateABXZ(int action, int part) {
//		switch(action) {
//		case MotionEvent.ACTION_DOWN:
//		case MotionEvent.ACTION_POINTER_DOWN:
//			switch(part) {
//			case 2: case 3: // define 'A' button
//				lastScan = SDL2.Scancode.X;
//				lastKey = SDL2.Keycode.x;
//				break;
//			case 4: case 5: // define 'B' button
//				lastScan = SDL2.Scancode.Z;
//				lastKey = SDL2.Keycode.z;
//				break;
//			case 6: case 7: // define 'Y' button
//				lastScan = SDL2.Scancode.A;
//				lastKey = SDL2.Keycode.a;
//				break;
//			case 1: case 8: // define 'X' button
//				lastScan = SDL2.Scancode.S;
//				lastKey = SDL2.Keycode.s;
//				break;
//			}
//			if(part >= 1 && part <= 8) {
//				this.sendKeyEvent(true, lastScan, lastKey, 0, 0);
//			}
//			break;
//		case MotionEvent.ACTION_UP:
//		case MotionEvent.ACTION_POINTER_UP:
//			if(lastScan != -1) {
//				sendKeyEvent(false, lastScan, lastKey, 0, 0);
//				lastKey = -1;
//				lastScan = -1;
//			}
//			break;
//		}
//	}
	
	@Override
	public void onPartitionEvent(View v, int action, int part) {
//		String obj = "null";
//		if(v == padLeft)	obj = "padLeft";
//		if(v == padRight)	obj = "padRight";
//		Log.d("ga_log", String.format("[%s] partition event: action=%d, part=%d", obj, action, part));
		// Left: emulated arrow keys
		if(v == padLeft) {
			emulateArrowKeys(action, part);
			return;
		}
		// Right: emulated ABXZ + mouse click
//		if(v == padRight) {
//			emulateABXZ(action, part);
//			return;
//		}
	}
	
	@Override
	public void onClick(View v) {
		if(v == buttonBack) {
			finish();
		} else if(v == buttonToggleLPad) {
			showLPad = !showLPad;
			this.onDimensionChange(currWidth, currHeight);
		} else if(v == buttonToggleRPad) {
			showRPad = !showRPad;
			this.onDimensionChange(currWidth, currHeight);
		} else if(v == buttonToggleLRBtn) {
			showLRBtn = !showLRBtn;
			this.onDimensionChange(currWidth, currHeight);
		}
	}

}
