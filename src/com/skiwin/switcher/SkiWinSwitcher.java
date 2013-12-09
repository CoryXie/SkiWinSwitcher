/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.skiwin.switcher;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;
import android.hardware.input.InputManager;

public class SkiWinSwitcher extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
		{
		super.onCreate(savedInstanceState);

		//startSkiWinJNI();

		}
	
	@Override
	public void onStart()
		{
		super.onStart();
		//resumeSkiWinJNI();
		startSkiWinJNI();		
		}
		
    @Override
	public void onResume()
		{
		super.onResume();
		resumeSkiWinJNI();
		}

	@Override
	public void onPause()
		{
		super.onPause();
		suspendSkiWinJNI();
		}

    /* A native method that is implemented by the
     * 'hello-jni' native library, which is packaged
     * with this application.
     */
    public native void startSkiWinJNI();
    public native void suspendSkiWinJNI();
    public native void resumeSkiWinJNI();

    /* this is used to load the 'SkiWinSwitcher' library on application
     * startup. The library has already been unpacked into
     * /data/data/com.skiwin.switcher/lib/libSkiWinSwitcher.so at
     * installation time by the package manager.
     */
    static {
        System.loadLibrary("SkiWinSwitcher");
    }
}
