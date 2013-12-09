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

pid_t skiwinPid = -1;

#define PROGRAM_NAME "/system/bin/SkiWin"

/*
One character from the string "RSDZTW" where  
R is running,  
S is sleeping in an interruptible wait, 
D is waiting in uninterruptible disk sleep,
Z is zombie,
T is traced or stopped (on a signal), 
W is paging.
*/

#define PROCESS_RUNNING                 'R'
#define PROCESS_WAIT_INTERRUPTABLE      'S'
#define PROCESS_WAIT_UNINTERRUPTABLE    'D'
#define PROCESS_ZOMBIE                  'Z'
#define PROCESS_TRACED_STOPPED          'T'
#define PROCESS_PAGING                  'W'

struct procstat { 
    int pid;               // %d 
    char comm[256];        // %s
    char state;            // %c
    };

/*

/proc/[number]/stat
          Status  information  about  the process. This is used by ps(1).
          It is defined in /usr/src/linux/fs/proc/array.c.

          The fields, in order, with their proper scanf(3) format speci-
          fiers, are:

          pid %d The process ID.

          comm %s
             The  filename of the executable, in parentheses.  This is
             visible whether or not the executable is swapped out.

          state %c
             One character from the string "RSDZTW" where  R is run-
             ning,  S is sleeping in an interruptible wait, D is wait-
             ing in uninterruptible disk sleep, Z is zombie, T is
             traced or stopped (on a signal), and W is paging.

*/
static char readStat(int pid) 
    { 
    const char *format = "%d %s %c"; 
    struct procstat stat;
    
    char buf[256]; 
    
    FILE *proc; 
    
    sprintf(buf,"/proc/%d/stat",pid); 
    
    proc = fopen(buf,"r"); 
    
    if (proc) 
        { 
        if (3 == fscanf(proc, format, &stat.pid, stat.comm, &stat.state)) 
            { 
            fclose(proc); 
            return stat.state; 
            } 
        else 
            { 
            fclose(proc); 
            return PROCESS_ZOMBIE; 
            } 
        } 
    else 
        {  
        return PROCESS_ZOMBIE; 
        } 
    }

void
Java_com_skiwin_switcher_SkiWinSwitcher_startSkiWinJNI(JNIEnv* env, jobject thiz)
{
if (skiwinPid == -1)
    {
    ALOGD("SSSSSSSSSSSSSSSSSSSSSSSSSSSSJava_com_skiwin_switcher_SkiWinSwitcher_startSkiWinJNI\n");
    pid_t pid=fork();
    if (pid==0) { /* child process */
        static char *argv[]={"echo","Foo is my name.",NULL};
        ALOGD("TTTTTTTTTTTTTTTTTTTT\n");
        execv(PROGRAM_NAME, argv);
        exit(127); /* only if execv fails */
        skiwinPid = -1;
        ALOGD("EEEEEEEEEEEEEEEEEEEE\n");
        }
    else { /* pid!=0; parent process */
        skiwinPid = pid;
        ALOGD("PPPPPPPPPPPPPPPPPPPP child=%d\n", skiwinPid);
        //waitpid(pid,0,0); /* wait for child to exit */
        }
    }

}

void
Java_com_skiwin_switcher_SkiWinSwitcher_suspendSkiWinJNI(JNIEnv* env, jobject thiz)
    {
    if (skiwinPid != -1)
        {
        ALOGD("KKKKKKKKKKKKKKKKKKKKKKKKKKKKJava_com_skiwin_switcher_SkiWinSwitcher_suspendSkiWinJNI\n");
        kill(skiwinPid, SIGSTOP);
        }
    }

void
Java_com_skiwin_switcher_SkiWinSwitcher_resumeSkiWinJNI(JNIEnv* env, jobject thiz)
    {
    if ((skiwinPid != -1) && (readStat(skiwinPid) == PROCESS_TRACED_STOPPED))
        {
        ALOGD("RRRRRRRRRRRRRRRRRRRRRRRRRRRRJava_com_skiwin_switcher_SkiWinSwitcher_resumeSkiWinJNI\n");

        kill(skiwinPid, SIGCONT);
        }
    else
        {
        ALOGD("RRRRRRRRRRRRRRRRRRRRRRRRRRRR===>Java_com_skiwin_switcher_SkiWinSwitcher_startSkiWinJNI\n");

        if ((skiwinPid != -1) && (readStat(skiwinPid) == PROCESS_ZOMBIE))
            {
            ALOGD("RRRRRRRRRRRRRRRRRRRRRRRRRRRR===>KILL PROCESS_ZOMBIE\n");
            kill(skiwinPid, SIGTERM);
            kill(skiwinPid, SIGKILL);
            waitpid(skiwinPid,0,0); /* wait for child to exit */
            skiwinPid = -1;
            }

        Java_com_skiwin_switcher_SkiWinSwitcher_startSkiWinJNI(env, thiz);
        }
    }

