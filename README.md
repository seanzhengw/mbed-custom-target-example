# Mbed custom target example

將這個函式庫加入專案內就可以使用這裡自訂的控制板。

這裡以 STM32F439ZI 自訂控制板為例，這個自訂板大多數的設計都是參考 NUCLEO-F439ZI

## 自訂版設定檔案

### custom_targets.json

這份檔案用於新增自訂板，這樣 mbed 編譯系統才能認到這個自訂板。

檔案內容可以參考 `mbed-os\targets\targets.json`

#### inherits 設定

`inherits` 的部分可以繼承內建開發板，像這裡的 MYBOARD 這樣可以直接使用內建的定義，同時也會有一些設定沒辦法更動。

若是比照 `mbed-os\targets\targets.json` 裡面的寫法，從 `"inherits": ["Target"]` 開始寫，能夠自訂的東西更多，同時要自己準備的檔案也就更多。

因為這裡是以 STM32F439ZI 為例，所以 MYBOARD2 的部分用 `"inherits": ["FAMILY_STM32"]`。

#### extra_labels 與 extra_labels_add 設定

另外 `extra_labels` 的部分會使 mbed 去使用 Target_XXX 內的檔案，例如 `"extra_labels":["ABC"]` 就會使 mbed 去使用 Target_ABC 資料夾內的檔案。

而 `extra_labels_add` 與 `extra_labels` 的功能相同，但是不會覆蓋繼承來的設定。

## 自訂板資料夾

只要將自料夾名稱取名為 TARGET_XXXXXX，mbed 就能判斷出這是屬於自訂板 XXXXXX 的資料夾。(或是 extra_labels XXXXXX 的資料夾)

這些自訂檔案都可以直接複製同型號或同系列 MCU 的檔案出來修改，例如 NUCLEO_F439ZI 的檔案路徑 `mbed-os\targets\TARGET_STM\TARGET_STM32F4\TARGET_STM32F439xI\TARGET_NUCLEO_F439ZI`

如果是從內建板繼承設定，那麼就只需要準備要取代的檔案即可。

如果是用 `"inherits": ["Target"]` 的方法的話就需要自己準備整份 Target 的檔案 (如 `mbed-os\targets\TARGET_STM`)

像這裡的 MYBOARD2 的部分用 `"inherits": ["FAMILY_STM32"]`，因為在 FAMILY_STM32 有設定 `"extra_labels":["STM"]`，所以會使用 `mbed-os\targets\TARGET_STM` 裡的檔案。

.h 檔部分會優先使用最底層的衍生 TARGET 提供的檔案，如自己提供了 PinNames.h，因為優先引用了，所以 include guard 會防止繼承的內建板的 PinNames.h 被重複引用。

.c 檔的部分則沒有辦法覆蓋，除非內建板有將函式加上 `__attribute__((weak))`

### PinNames.h 

幫板上的腳位定名稱，要注意有些名稱 mbed 會使用到，不能因為自訂板上沒有該腳位就完全不定義。

以 NUCLEO-F439ZI 可以參考 `mbed-os\targets\TARGET_STM\TARGET_STM32F4\TARGET_STM32F439xI\TARGET_NUCLEO_F439ZI`

### PeripheralPins.c 與 PeripheralPinMaps.h

各腳位對應的功能，可以參考內建的寫法，並調整成適合自訂板的內容。

只要比照內建板的檔案寫法，通常不會遇到問題。

以 NUCLEO-F439ZI 為例，參考 `mbed-os\targets\TARGET_STM\TARGET_STM32F4\TARGET_STM32F439xI\TARGET_NUCLEO_F439ZI`

### system_clock.c

設定系統頻率。

這部分程式會由 mbed 去調用，所以必須比照內建板檔案的函式名稱，內建板的函式通常會將系統頻率設定為最高。

以 NUCLEO-F439ZI 可以參考 `mbed-os\targets\TARGET_STM\TARGET_STM32F4\TARGET_STM32F439xI\TARGET_NUCLEO_F439ZI`

### 其他檔案

* `targets\TARGET_MYBOARD2\stm32f4_eth_init.c`: 關於 EMAC，參見 `mbed-os\features\netsocket\emac-drivers`
* `targets\TARGET_MYBOARD2\stm32f4_eth_conf.c`: 關於 EMAC，參見 `mbed-os\features\netsocket\emac-drivers`
* `targets\TARGET_MYBOARD2\mbedtls_device.h`: 內有關於 TLS (傳輸安全)，參見 `mbed-os\features\mbedtls\targets`

## 自訂板總結

其實只有一個原則，只要 XXXX 不是板子名稱或 `extra_labels` 設定內提到的關鍵字，那個 TARGET_XXXX 就會被 mbed 編譯系統忽略。

如果是打算完全從零開始建立自訂板(從 `"inherits": ["Target"]` 開始寫)，那麼在 mbed-os 子路徑內 TARGET_ 開頭路徑內的檔案全部都要準備一份，若是自訂板不支援某個函式庫，也要在 custom_targets.json 中設定。

# 編譯

使用 Mbed CLI  (Mbed Online Compiler 不支援自訂板)

編譯程式時使用 `-m` 指定開發板，並用 `--custom-targets` 指定本函式庫的 `custom_targets.json` 所在路徑，如

    mbed compile -m MYBOARD --custom-targets mbed-custom-target-example

# 授權

以下這些檔案複製而來，這些檔案採用的授權如下列表

* `targets/TARGET_MYBOARD2/PeripheralNames.h` BSD-3-Clause
* `targets/TARGET_MYBOARD2/PeripheralPins.c` BSD-3-Clause
* `targets/TARGET_MYBOARD2/stm32f4_eth_conf.c` Apache 2.0
