/*
 * Copyright 2017 Google Inc. All Rights Reserved.
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

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <jni.h>

#include <airmap/context.h>
#include <airmap/monitor/client.h>
#include <airmap/util/formatting_logger.h>

extern "C" {

  namespace {
      // maintain a reference to the JVM so we can use it later.
      static JavaVM *g_vm = nullptr;
  }

jint JNI_OnLoad(JavaVM *vm, void *) {
  g_vm = vm;
  return JNI_VERSION_1_6;
}

JNIEXPORT jlong JNICALL
Java_com_airmap_airmapd_JniInterface_createNativeApplication
(JNIEnv *env, jclass, jobject j_asset_manager, jobject context) {
  AAssetManager *asset_manager = AAssetManager_fromJava(env, j_asset_manager);
  return 0;
}

JNIEXPORT void JNICALL Java_com_airmap_airmapd_JniInterface_destroyNativeApplication
(JNIEnv *, jclass, jlong native_application) {
}

JNIEXPORT void JNICALL Java_com_airmap_airmapd_JniInterface_onPause
(JNIEnv *, jclass, jlong native_application) {
}

JNIEXPORT void JNICALL Java_com_airmap_airmapd_JniInterface_onResume
(JNIEnv *, jclass, jlong native_application) {
}

JNIEXPORT void JNICALL Java_com_airmap_airmapd_JniInterface_onGlSurfaceCreated
(JNIEnv *, jclass, jlong native_application) {
}

JNIEXPORT void JNICALL Java_com_airmap_airmapd_JniInterface_onDisplayGeometryChanged
(JNIEnv *, jclass, jlong native_application, int display_rotation, int width,
 int height) {
}

JNIEXPORT void JNICALL Java_com_airmap_airmapd_JniInterface_onGlSurfaceDrawFrame
(JNIEnv *, jclass, jlong native_application) {
}

JNIEXPORT void JNICALL Java_com_airmap_airmapd_JniInterface_onTouched
(JNIEnv *, jclass, jlong native_application, jfloat x, jfloat y) {
}

JNIEXPORT jboolean JNICALL Java_com_airmap_airmapd_JniInterface_hasDetectedPlanes
(JNIEnv *, jclass, jlong native_application) {
  return static_cast<jboolean>(true);
}

JNIEnv *GetJniEnv() {
  JNIEnv *env;
  jint result = g_vm->AttachCurrentThread(&env, nullptr);
  return result == JNI_OK ? env : nullptr;
}

jclass FindClass(const char *classname) {
  JNIEnv *env = GetJniEnv();
  return env->FindClass(classname);
}

}  // extern "C"
