## 需求
- c++ compiler
- cmake
- opencv

## 編譯

採用cmake作為自動化工具
```
# 在project根目錄
mkdir build
cd build
cmake .. # 生成Makefile
make
```
然後就能得到執行檔 align

## 目前使用方式
```
align image1 image2 image3 .....
```

## 問題

當照片過暗時，中位數之強度為0，故根本無法辨識，致使對齊不可能，可能的解決法為將分隔設為四分之一
