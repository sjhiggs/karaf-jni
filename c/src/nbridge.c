/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#define _GNU_SOURCE

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/auxv.h>
#include <sys/syscall.h>

#include "com_rh_example_NativeBridge.h"

JNIEXPORT void JNICALL Java_com_rh_example_NativeBridge_helloNative
    (JNIEnv *env, jobject obj, jstring msgFromJava) {

    const char *s = (*env)->GetStringUTFChars(env, msgFromJava, NULL);
    printf("native code received: %s\n", s);

    jclass cls = (*env)->GetObjectClass(env, obj);

    //specify the helloCallback method from the calling Java object, with a method that
    //takes a String parameter and returns void
    jmethodID mid = (*env)->GetMethodID(env, cls, "helloCallback", "(Ljava/lang/String;)V");

    if (mid == 0) {
        printf("ERROR!  Java callback method incorrect!\n");
        return;
    }

    jstring jstr = (*env)->NewStringUTF(env, "native code says hello");
    (*env)->CallVoidMethod(env, obj, mid, jstr);

}
