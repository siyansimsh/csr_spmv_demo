#include <stdio.h>
#include <stdlib.h>

/*
  使用 CSR (Compressed Sparse Row) 格式做 SpMV (Sparse Matrix-Vector multiply)
  y = A * x

  CSR 三個陣列：
    - row_ptr: 長度 = nrows + 1。第 i 列的非零元素在 val/col 中的範圍是
               [ row_ptr[i], row_ptr[i+1] )，左閉右開。
    - col    : 與 val 同長度 (= 非零數 nnz)，col[j] 是第 j 個非零元素的「欄索引」。
    - val    : 與 col 同長度 (= nnz)，val[j] 是第 j 個非零元素的「數值」。
*/
void spmv_csr(int nrows,
              const int *row_ptr, const int *col, const double *val,
              const double *x, double *y)
{
    // 逐列把第 i 列的非零拿出來，累加到 y[i]
    for (int i = 0; i < nrows; ++i) {
        double sum = 0.0;
        // 第 i 列在 CSR 的區間： [row_ptr[i], row_ptr[i+1])
        for (int j = row_ptr[i]; j < row_ptr[i + 1]; ++j) {
            // 第 j 個非零在 (i, col[j]) 位置，數值為 val[j]
            // 其對 y[i] 的貢獻 = val[j] * x[col[j]]
            sum += val[j] * x[col[j]];
        }
        y[i] = sum;
    }
}

int main(void)
{
    const int n = 10;              // 矩陣為 n × n (本例 n=10)

    /*
      建一個 n×n 的三對角矩陣：
        主對角 =  2
        上對角 = -1
        下對角 = -1

      非零數 nnz = 3*n - 2
        - 首列缺下對角、末列缺上對角，所以比 3n 少 2。
    */
    const int nnz = 3 * n - 2;

    // 配三個 CSR 陣列
    double *val    = (double *)malloc(nnz * sizeof(double));
    int    *col    = (int    *)malloc(nnz * sizeof(int));
    int    *rowptr = (int    *)malloc((n + 1) * sizeof(int));

    if (!val || !col || !rowptr) {
        fprintf(stderr, "malloc failed\n");
        return 1;
    }

    // 依「列」把三對角矩陣塞進 CSR
    int k = 0;              // 掃描 val/col 的寫入位置
    rowptr[0] = 0;          // CSR 規則：row_ptr[0] 一定是 0
    for (int i = 0; i < n; ++i) {
        if (i - 1 >= 0) {          // 下對角：A[i, i-1] = -1
            val[k] = -1.0; col[k] = i - 1; ++k;
        }
        {                           // 主對角：A[i, i]   =  2
            val[k] =  2.0; col[k] = i;     ++k;
        }
        if (i + 1 < n) {           // 上對角：A[i, i+1] = -1
            val[k] = -1.0; col[k] = i + 1; ++k;
        }
        // 下一列的起點（同時也是這一列的結束索引，上界 exclusive）
        rowptr[i + 1] = k;
    }

    // 測試向量 x = [1, 2, ..., 10]^T
    double x[10], y[10];
    for (int i = 0; i < n; ++i) x[i] = (double)(i + 1);

    // 做 SpMV
    spmv_csr(n, rowptr, col, val, x, y);

    // 印結果
    printf("y = A * x :\n");
    for (int i = 0; i < n; ++i) {
        printf("y[%d] = %g\n", i, y[i]);
    }

    // 額外：把 CSR 內部陣列印出來以利檢查
    printf("\nCSR arrays:\nrow_ptr: ");
    for (int i = 0; i <= n; ++i) printf("%d ", rowptr[i]);
    printf("\ncol   : ");
    for (int i = 0; i < nnz; ++i) printf("%d ", col[i]);
    printf("\nval   : ");
    for (int i = 0; i < nnz; ++i) printf("%g ", val[i]);
    printf("\n");

    // 清理記憶體
    free(val);
    free(col);
    free(rowptr);
    return 0;
}
