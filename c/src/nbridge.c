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

typedef struct thread_args {
    JavaVM *vm;
    jobject obj;
    jstring msgFromJava;
} thread_args;

void *callback(void *callbackArgs) {

    thread_args *args = (thread_args *)callbackArgs;
    JavaVM *vm = args->vm;
    void *penv;

    (*vm)->AttachCurrentThread(vm, &penv, NULL);
    JNIEnv *env = (JNIEnv *)penv;

    const char *s = (*env)->GetStringUTFChars(env, args->msgFromJava, NULL);
    printf("native code received: %s\n", s);

    jclass callbackClazz = (*env)->FindClass(env, "Lcom/rh/example/NativeBridgeCallback;");
    jmethodID callbackMid = (*env)->GetMethodID(env, callbackClazz, "doCallback", "(Ljava/lang/String;)V");
    if (callbackMid == 0) {
        printf("ERROR!  Java callback method incorrect!\n");
        (*vm)->DetachCurrentThread(vm);
        return NULL;
    }
    jmethodID constructorMid = (*env)->GetMethodID(env, callbackClazz, "<init>", "()V");
    if (constructorMid == 0) {
        printf("ERROR!  Java constructor method incorrect!\n");
        (*vm)->DetachCurrentThread(vm);
        return NULL;
    }

    jstring jstr = (*env)->NewStringUTF(env, "native code says hello");
    jobject newObj = (*env)->NewObject(env, callbackClazz, constructorMid);
    (*env)->CallVoidMethod(env, newObj, callbackMid, jstr);

    (*vm)->DetachCurrentThread(vm);
}

JNIEXPORT void JNICALL Java_com_rh_example_NativeBridge_helloNative
    (JNIEnv *env, jobject obj, jstring msgFromJava) {

    printf("start native\n");

    thread_args myargs;
    myargs.obj = obj;
    myargs.msgFromJava = msgFromJava;
    (*env)->GetJavaVM(env, &myargs.vm);

    pthread_t mythread;

    if(pthread_create(&mythread, NULL, callback, (void *)&myargs)) {
        fprintf(stderr, "Error creating thread\n");
        return;
    }

    if(pthread_join(mythread, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return;
    }

    return;
}
