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
 *
 */
#include <string.h>
#include <jni.h>
#include <utils/Log.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/hellojni/HelloJni.java
 */
jstring
Java_com_skiwin_switcher_SkiWinSwitcher_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{
	#if 0
    /*Spawn a child to run the program.*/
    pid_t pid=fork();
    if (pid==0) { /* child process */
        static char *argv[]={"echo","Foo is my name.",NULL};
		ALOGD("xxxxDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\n");
        execv("/system/bin/SkiWin",argv);
        exit(127); /* only if execv fails */
    }
    else { /* pid!=0; parent process */
        waitpid(pid,0,0); /* wait for child to exit */
    }
	#else
 	system("/system/bin/SkiWin");
	#endif
    return (*env)->NewStringUTF(env, "Hello from JNI2 !");
}
