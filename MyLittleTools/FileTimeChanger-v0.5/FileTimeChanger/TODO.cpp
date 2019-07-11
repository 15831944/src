/*
already——遍历CFileList并形成一个vector表,拖拽添加，del删除,change time

1. 控制time change逻辑：a2t与t2a不能同时进行，CT时间<=AT<=MT
2. 添加时间检测，CT时间<=MT<=AT

进阶功能：

1. FileList右键菜单支持
2. 使用Tree显示文件夹数据,只获取当前可以看到的文件
3. 添加从Tree直接拖拽文件到FileList的功能
4. 添加拖拽文件到FileList的功能
5. 文件夹支持
*/
#include "stdafx.h"