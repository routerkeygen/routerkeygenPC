Router Keygen generate default WPA/WEP keys for the following router:

  # Thomson based routers ( this includes Thomson, SpeedTouch, Orange, Infinitum, BBox, DMax, BigPond, O2Wireless, Otenet, Cyta , TN_private, Blink )
  # DLink ( only some models )
  # Pirelli Discus
  # Eircom
  # Verizon FiOS ( only some routers supported)
  # Alice AGPF
  # FASTWEB Pirelli and Telsey
  # Huawei (some InfinitumXXXX)
  # Wlan_XXXX or Jazztel_XXXX
  # Wlan_XX ( only some are supported)
  # Ono ( P1XXXXXX0000X )
  # WlanXXXXXX, YacomXXXXXX and WifiXXXXXX
  # Sky V1 routers
  # Clubinternet.box v1 and v2 ( TECOM-AH4XXXX )
  # InfostradaWifi
  # CONN-X
  # Megared
  # EasyBox, Arcor and Vodafone
  # PBS (Austria)
  # MAXCOM
  # PTV
  # TeleTu/Tele2
  # Axtel, Axtel-xtremo
  # Intercable
  # OTE
  # Cabovisao Sagem
  # Alice in Germany *(new)*
  # Speedport *(new)*

Building


Windows (Cross Compilation from Ubuntu)


Get MXE
make qt openssl
run build_win.sh

Mac OS X

Define CMAKE_PREFIX_PATH to point to your Qt instalation.
Example: /Users/awesome/Qt/5.3.0/clang_64

run build_mac.sh

Linux

Ubuntu
sudo apt-get install  cmake build-essential qt5-default libssl-dev qtscript5-dev libnm-gtk-dev qttools5-dev

run build_linux.sh

