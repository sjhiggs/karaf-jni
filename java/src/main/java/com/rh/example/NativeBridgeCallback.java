package com.rh.example;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class NativeBridgeCallback {

    public void doCallback(String msg) {
        Logger LOG = LoggerFactory.getLogger(NativeBridgeCallback.class);
        LOG.info("Java received message from native code: " + msg);
    }
}
