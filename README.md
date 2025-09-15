# CSR SpMV Demo (C)

這個專案示範如何用 **CSR (Compressed Sparse Row)** 格式儲存稀疏矩陣，並進行 **SpMV**（Sparse Matrix × Vector）運算：`y = A * x`。

## 內容
- `csr_spmv_demo.c`：建立一個 10×10 的三對角稀疏矩陣：
  - 主對角 = 2、上/下對角 = -1
  - 測試向量 `x = [1, 2, ..., 10]^T`
  - 使用 CSR 三陣列 `row_ptr / col / val` 表示 A，並計算 `y = A * x`
- 輸出：
  - `y` 的每個分量
  - CSR 內部三個陣列，便於驗證

## CSR 格式速記
- `row_ptr`：長度為 `nrows + 1`。第 `i` 列非零在 `[row_ptr[i], row_ptr[i+1])`。
- `col`：與 `val` 等長，記錄每個非零的 **欄索引**。
- `val`：記錄每個非零的 **值**。

> **不變式**：`row_ptr[0] = 0`、`row_ptr[nrows] = nnz`。

## 編譯與執行

### Windows（MSYS2/MinGW-w64）
```powershell
# 進到原始碼資料夾
cd C:\c_codes\C

# 編譯
C:\msys64\mingw64\bin\gcc.exe csr_spmv_demo.c -O2 -Wall -Wextra -o csr_spmv_demo.exe

# 執行
.\csr_spmv_demo.exe
