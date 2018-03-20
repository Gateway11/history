#!/bin/bash

./signer/apksigner sign --key signer/security/platform.pk8 --cert signer/security/platform.x509.pem bin/rokid-openvoice-process-android-ndk.apk
adb install -r bin/rokid-openvoice-process-android-ndk.apk
