# Notes


# Tips
下载单独一个文件夹
```shell
# 1
git clone --filter=blob:none --sparse git@github.com:qujihan/notes.git
# 或者使用 git clone --filter=blob:none --sparse https://github.com/qujihan/notes.git

# 2
# 以下载 rust/ds 文件夹为例
git sparse-checkout set rust/ds
```