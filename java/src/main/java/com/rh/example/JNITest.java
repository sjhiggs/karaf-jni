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

package com.rh.example;

import java.util.Map;

import org.osgi.framework.BundleContext;
import org.osgi.service.component.ComponentContext;
import org.osgi.service.component.annotations.Activate;
import org.osgi.service.component.annotations.Component;
import org.osgi.service.component.annotations.Deactivate;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

@Component(name = "com.rh.example.karaf-jni", immediate = true, enabled = true)
public class JNITest {

    private static final Logger LOG = LoggerFactory.getLogger(JNITest.class);

    static {
        //System.load("/opt/native-bridge/libnbridge.so");
        System.loadLibrary("nbridge");
    }

    @Activate
    public void activate(ComponentContext c, BundleContext b, Map<String, Object> props) {
        LOG.info("starting");
        new NativeBridge().testNative();
    }

    @Deactivate
    public void deactivate(ComponentContext c, BundleContext b, Map<String, Object> props) {
        LOG.info("stopping");
    }


    public static void main(String[] args) {
        new NativeBridge().testNative();
    }
}
