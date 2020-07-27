# Credits
Thanks to  grgrzybek, this project is based on:

https://github.com/grgrzybek/f2f2019/

# Java OSGi with JNI

This project demonstrates usage of JNI with Karaf.  The use case is:

1.  Karaf (Java) bundle is installed
2.  Karaf bundle calls Native C Code
3.  Native C Code calls back into the Karaf bundle

# Test Environment

The following versions were used during development/testing:

* Red Hat Fuse 7.6.0 (Karaf/OSGi container)
* Openjdk 1.8
* Fedora 32

# Building/Installing

## C Project

From the 'c' project, execute:
```
$ mkdir build
$ cd build/
$ cmake ../
$ cmake --build .
```

## Copy shared library to Java project

Typically, the Java project can simply call System.load with the shared library location. In the Karaf/OSGi environment, the following will happen if the OSGi bundle is uninstalled/installed:

```
The activate method has thrown an exception
java.lang.UnsatisfiedLinkError: Native Library /opt/native-bridge/libnbridge.so already loaded in another classloader
```

To avoid this, the native library is included in the bundle itself, and the bundle's headers are modified accordingly.

Copy the share library to the Java project so it's available during the next step (build/install Java Project):

```
$ cp c/build/libnbridge.so java/src/main/resources/
```

See the pom.xml for the bundle headers, which make it easier to load the library from the bundle:
```
<Bundle-NativeCode>libnbridge.so;osname=linux;processor=x86-64</Bundle-NativeCode>
```

## Java Project
From the Java project, execute:
```
$ mvn clean install
```

## Install/Run

```
karaf@root()> osgi:install -s mvn:com.rh.example/karaf-jni/1.0.0
```

Observe the following logs from stdout and the logs:

C code writes the following to stdout
```
native code received: hello from karaf
```

Java code, in the callback, writes the following to the logs:
```
INFO [pipe-bundle:install $args] native code says hello
```

## Re-generate JNI header (Optional)

If the JNI bridge class changes and a new C header is needed, re-generate the header from the base project directory:

```
$ javah -cp java/target/classes/ -d c/include/ com.rh.example.NativeBridge
```
