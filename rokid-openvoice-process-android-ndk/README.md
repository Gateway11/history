# 使用方法

首先请看 [ndk 使用指南](https://developer-forum.rokid.com/t/topic/97)

# 自定义激活词

自动编译后会生成Android 标准应用程序: system/priv-app/openvoice_process/openvoice_process.apk

同理如下⽬录下拿到 BearKid Jar, 此 Jar 包获取 ASR、NLP 消息，自定义激活词接口等
```
out/target/common/obj/JAVA_LIBRARIES/BearKidAdapter_intermediates/classes.jar
```

如在 905d 开发板上，要开始使用自定义激活词，首先得先在您的设备上加载openvoice_process.apk，命令如下
```
adb root
adb remount
adb shell mkdir /system/priv-app/openvoice_process/
adb push openvoice_process.apk /system/priv-app/openvoice_process/
reboot
```
## 接口使用注意事项

1. 在您的 libs 添加 jar 包 BearKid Jar
2. 需要 import 以下三个类

```
import com.rokid.voicerec.BearKidAdapter;
import com.rokid.voicerec.BearKidCallback;
import com.rokid.voicerec.CustomWord;
```

## API

```c
int addCustomWord(int type, String word, String pinyin);
// 添加自定义激活词接口
// 参数type: 有3种 TYPE_VT_WORD (唤醒激活词), TYPE_EXIT_WORD (推出激活词), TYPE_HOT_WORD (热词激活词）
// 参数word: 激活词中文文字   如:小月亮
// 参数pinyin: 激活词拼音+声调 如:xiao3yue4liang4

int removeCustomWord(String word);
// 删除自定义激活词接口,参数 word: 激活词中文文字

List<CustomWord> queryCustomWord(int type);
// 查询自定义激活词接口，参数type：同上
```

# ASR 使用方法

同理如下⽬录下拿到 BearKid Jar, 此 Jar 包获取 ASR、NLP 消息，自定义激活词接口等
```
out/target/common/obj/JAVA_LIBRARIES/BearKidAdapter_intermediates/classes.jar
```
## 接口使用注意事项 
1. 在您的 libs 添加 jar 包 BearKid Jar
2. 需要 import 以下三个类

```
import com.rokid.voicerec.BearKidAdapter;
import com.rokid.voicerec.BearKidCallback;
import com.rokid.voicerec.CustomWord;
```

## API
```c
void onVoiceEvent(int event, int arg1, double arg2);
// 'event': EVENT_LOCATION   'arg1' 无意义  'arg2' 语音方向信息
//          EVENT_VOICE_INFO 'arg1' 无意义  'arg2' 音强信息
//          EVENT_ACTIVATION 'arg1' 接受激活或拒绝激活 'arg2' 无意义
//          EVENT_DEACTIVATE 'arg1' 无意义  'arg2' 无意义

void onIntermediateResult(String asr);
// 中间asr识别结果

void onRecognizeResult(String asr, String nlp, String action);
// 'asr'不为空  整句话的asr识别结果。此时'nlp', 'action'一定为空
// 'nlp', 'action'不为空  语义理解结果。此时'asr'一定为空

void onException(int exception);
// 'exception' 异常类型
```

## 示例

```c
package com.example.demo;

import android.app.Activity;
import android.os.Bundle;

import com.rokid.voicerec.BearKidAdapter;
import com.rokid.voicerec.BearKidCallback;
import com.rokid.voicerec.CustomWord;

public class MainActivity extends Activity implements BearKidCallback {

    public static final String TAG = "BEAR_KID_ACTIVITY";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        new Thread(new Runnable() {
            @Override
            public void run() {
                BearKidAdapter bearKidAdapter = new BearKidAdapter();
                boolean initialize = bearKidAdapter.initialize(getApplicationContext(), "com.rokid.openvoice.VoiceService", MainActivity.this);
                int i = bearKidAdapter.addCustomWord(CustomWord.TYPE_VT_WORD, "小月亮", "xiao3yue4liang4");
            }
        }).start();
    }

    @Override
    public void onVoiceEvent(int i, int i1, double v) {

    }

    @Override
    public void onIntermediateResult(String s) {

    }

    @Override
    public void onRecognizeResult(String s, String s1, String s2) {

    }

    @Override
    public void onException(int i) {

    }
}

```
## 注意事项
1. 配网方法请参照 (android开发板烧写流程的配网环节)[https://developer-forum.rokid.com/t/topic/991]

2. 修改/system/etc/openvoice_proc.json 对应字段（配置openvoice 对应 key ID)[https://developer-forum.rokid.com/t/topic/991]

