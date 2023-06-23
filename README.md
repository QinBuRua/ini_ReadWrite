# ini_ReadWrite

## zh-CN:

使用：

### 创建：

```c_cpp
_iniFile ini;         //在内存创建一个空ini文件
_iniFile ini("fileName");   //在内存创建一个ini文件并读取"fileName"
```

### 使用：

读取：

```c_cpp
int value=ini.get("section","key");
int value=ini["section"]["key"];
//支持 double,int,string,bool 的直接取值。
```

获取值，其中，get()执行检查，检查键是否存在，不存在则返回空值；[][]不执行检查，直接返回值，键不存在会出现不可预测结果。

赋值：

```c_cpp
ini.set("section","key","value");
//支持 double,int,string,bool 的直接赋值。
```

展示与写入：

```c_cpp
ini.show();           //显示在控制台上
ini.save("fileName"); //写入文件
```
