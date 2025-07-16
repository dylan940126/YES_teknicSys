#!/bin/bash

# 設置庫路徑
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/dylan/data/myclone/YES_teknicSys/ver_2/Dependencies/linux/sFoundation/sFoundation:/home/dylan/data/myclone/YES_teknicSys/ver_2/Dependencies/common/ADS/build/AdsLib

# 進入程序目錄
cd /home/dylan/data/myclone/YES_teknicSys/ver_2/build || { echo "錯誤: 無法進入程序目錄"; exit 1; }

# 運行主程序
./main
