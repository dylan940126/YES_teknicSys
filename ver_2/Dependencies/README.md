# 依賴項管理

本目錄包含專案所需的所有第三方庫和依賴項。依賴項按平台進行組織。

## 目錄結構

- `common/`: 跨平台共用的庫
  - `eigen-3.3.7/`: Eigen 線性代數庫

- `windows/`: Windows 平台專用庫
  - `sFoundation20/`: Teknic ClearPath 馬達控制庫（Windows 版）
  - `TcAdsDll/`: Beckhoff TwinCAT ADS 通訊庫（Windows 版）
  - `wxMSW/`: wxWidgets GUI 庫（Windows 版）

- `linux/`: Linux 平台專用庫
  - `ADS/`: Beckhoff ADS 通訊庫（Linux 版）
  - `sFoundation/`: Teknic ClearPath 馬達控制庫（Linux 版）
  - `DynamixelSDK/`: Dynamixel 馬達 SDK

## 新增依賴項

如需新增依賴項，請按照以下步驟操作：

1. 確定依賴項是特定於某個平台還是跨平台共用
2. 在相應的子目錄中創建適當的文件夾
3. 更新 CMakeLists.txt 以包含新的依賴項

## 編譯依賴項

某些依賴項需要在使用前編譯。請參考專案根目錄的 README.md 以獲取具體指示。

### ADS 庫（Linux）

```bash
cd common/ADS
mkdir -p build
cd build
cmake ..
make -j$(nproc)
```

### sFoundation 庫（Linux）

```bash
cd linux/sFoundation/sFoundation
make
```
