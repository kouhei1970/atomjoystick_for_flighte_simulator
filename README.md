# Joystick for StampFly Simulator

## PlatformIO
ビルド環境の準備

```
cd
mkdir -p tmp/pio
cd tmp/pio
wget -O get-platformio.py https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py
python3 get-platformio.py
```

## Install

ATOMS3をUSBケーブルでPCに接続する. リセットボタンを緑のLEDの点灯を確認するまで長押しする

```
cd
. .platformio/penv/bin/activate
mkdir -p tmp/github
cd tmp/github
git clone https://github.com/kouhei1970/atomjoystick_for_flighte_simulator.git
cd atomjoystick_for_flighte_simulator
pio run --target upload
```