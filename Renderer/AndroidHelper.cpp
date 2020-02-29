#include <LogUtils.h>

#ifdef __ANDROID__

#define LOG_NDEBUG 0

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "AndroidHelper"
#endif

#include "AndroidHelper.h"

shared_ptr<MKitHelper> MKitHelper::getInstance() {
	if (instance == nullptr) {
		instance = shared_ptr<AndroidHelper>(new AndroidHelper());
	}
	return instance;
}

void AndroidHelper::initJavaVariables(JavaVM * javaVM, jobject activityObject, jobject jniHelperObject) {
	this->javaVM = javaVM;

	JNIEnv *env;
	if (javaVM->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
	}

	this->activityObject = reinterpret_cast<jobject>(env->NewGlobalRef(activityObject));

	jclass jniHelperClass = env->FindClass("com/maxst/mkit/MKitJniHelper");
	if (jniHelperClass == 0) {
		return;
	}

	this->jniHelperObject = reinterpret_cast<jobject>(env->NewGlobalRef(jniHelperObject));

	onClickMethodId = env->GetMethodID(jniHelperClass, "onClick", "(Ljava/lang/String;I)V");
	if (onClickMethodId == 0) {
		return;
	}

	onCheckedChangedMethodId = env->GetMethodID(jniHelperClass, "onCheckedChanged", "(Ljava/lang/String;Z)V");
	if (onClickMethodId == 0) {
		return;
	}

	onTrackingFailMethodId = env->GetMethodID(jniHelperClass, "onTrackingFail", "()V");
	if (onTrackingFailMethodId == 0) {
		LOGD("onTrackingFail method not found");
		return;
	}

	onTrackingSuccessMethodId = env->GetMethodID(jniHelperClass, "onTrackingSuccess", "(Ljava/lang/String;Ljava/lang/String;)V");
	if (onTrackingSuccessMethodId == 0) {
		LOGD("onTrackingSuccess method not found");
		return;
	}

	onEventMethodId = env->GetMethodID(jniHelperClass, "onEvent", "(I)V");
	if (onEventMethodId == 0) {
		LOGD("onEvent method not found");
		return;
	}
}

void AndroidHelper::launchUrl(const char *url, bool isWebView) {
	printf("AndroidHelper::launchUrl");
	JNIEnv *env;

	if (javaVM->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
		return;
	}

	jstring jstrUrl = env->NewStringUTF(url);

	jclass classActivity = env->FindClass("android/app/Activity");
	if (classActivity == 0) {
		return;
	}
	jclass classIntent = env->FindClass("android/content/Intent");
	if (classIntent == 0) {
		return;
	}
	jclass classUri = env->FindClass("android/net/Uri");
	if (classUri == 0) {
		return;
	}

	jfieldID fieldActionView = env->GetStaticFieldID(classIntent, "ACTION_VIEW", "Ljava/lang/String;");
	if (fieldActionView == 0) {
		return;
	}
	jstring paramActionView = (jstring) env->GetStaticObjectField(classIntent, fieldActionView);
	if (paramActionView == 0) {
		return;
	}
	jmethodID methodUriParse = env->GetStaticMethodID(classUri, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
	if (methodUriParse == 0) {
		return;
	}
	jmethodID methodActivityStartActivity = env->GetMethodID(classActivity, "startActivity", "(Landroid/content/Intent;)V");
	if (methodActivityStartActivity == 0) {
		return;
	}
	jmethodID methodIntentInit = env->GetMethodID(classIntent, "<init>", "(Ljava/lang/String;Landroid/net/Uri;)V");
	if (methodIntentInit == 0) {
		return;
	}

	jobject paramUri = env->CallStaticObjectMethod(classUri, methodUriParse, jstrUrl);
	if (paramUri == 0) {
		return;
	}
	jobject paramIntent = env->NewObject(classIntent, methodIntentInit, paramActionView, paramUri);
	if (paramIntent == 0) {
		return;
	}
	env->CallVoidMethod(activityObject, methodActivityStartActivity, paramIntent);
}

void AndroidHelper::onTrackingFail() {
	JNIEnv *env;

	if (javaVM->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
		return;
	}

	env->CallVoidMethod(jniHelperObject, onTrackingFailMethodId);
}

void AndroidHelper::onTrackingSuccess(const char * trackableId, const char * trackableName) {
	JNIEnv *env;

	if (javaVM->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
		return;
	}

	jstring id = env->NewStringUTF((char* )trackableId);
	jstring name = env->NewStringUTF((char* )trackableName);

	env->CallVoidMethod(jniHelperObject, onTrackingSuccessMethodId, id, name);
}

void AndroidHelper::onClick(const char* controlId, int resId) {
	LOGD("AndroidHelper::onClickEvent");
	JNIEnv *env;

	if (javaVM->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
		return;
	}

	jstring jstrControlId = env->NewStringUTF(controlId);
	env->CallVoidMethod(jniHelperObject, onClickMethodId, jstrControlId, resId);
}

void AndroidHelper::onCheckedChanged(const char * controlId, bool isChecked) {
	LOGD("AndroidHelper::onCheckedChanged");
	JNIEnv *env;

	if (javaVM->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
		return;
	}

	jstring jstrControlId = env->NewStringUTF(controlId);
	env->CallVoidMethod(jniHelperObject, onCheckedChangedMethodId, jstrControlId, (jboolean)isChecked);
}

void AndroidHelper::onEvent(int eventId) {
	LOGD("AndroidHelper::onEvent");
	JNIEnv *env;

	if (javaVM->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
		return;
	}

	env->CallVoidMethod(jniHelperObject, onEventMethodId, eventId);
}

void AndroidHelper::clear() {
	MKitHelper::clear();

	if(javaVM == nullptr) //not initialized
	{
		return;
	}

	JNIEnv *env;
	if (javaVM->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
		return;
	}

	env->DeleteGlobalRef(activityObject);
	env->DeleteGlobalRef(jniHelperObject);
	activityObject = 0;
	jniHelperObject = 0;
}

#endif // __ANDROID__

