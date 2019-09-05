---
title: 基于vs2019ide,win10系统下自身opencv的部署
tags: 配置
date: 2019-07-30 10:11:39
categories: opencv
description: opencv项目在vs2019win系统的部署自身配置过程
Mathjax:
---

由于部署配置比较麻烦,而且容易忘记,所以写这个blog来方便自己以后再次进行配置

1. 修改文件夹名字
2. 项目名称(google翻译)
3. 新建项目
4. 控制台应用->下一步
5. 位置:\\Mac\Home\Desktop\github\opencv_practice\charpterx\xx\xx
6. 打开源代码的文件,把源代码的源图(附件)copy到opencv_practice里面
7. 工具栏上选择Debug配置x64平台
8. 导航栏选择调试->xx属性
9. v/c++目录:包含目录添加  C:\software\opencv\opencv\build\include   C:\software\opencv\opencv\build\include\opencv2
10. v/c++目录:包含目录添加  C:\software\opencv\opencv\build\x64\vc15\lib
11. 连接器->输入:附加依赖项添加   opencv_world346d.lib
12. 关闭当前项目源代码ide
13. 打开下一个源代码文件
14. 弹出的安全警告把```对于此解决方案的每一个项目都询问我的勾去掉```
15. 检查解决方案直接点确定
16. push
17. 删除.vs文件(解决方案太大放不下GitHub)
18. 重复1-16

opencv_world346d.lib

> 其中，exp为实验内容，charpter为书籍实战

>my blog: [https://zyhang8.github.io/2019/07/30/opencv-vs2019-win-config/](https://zyhang8.github.io/2019/07/30/opencv-vs2019-win-config/)

<blockquote style="margin: 2em 0 0;padding: 0.5em 1em;border-left: 3px solid #F44336;background-color: #F5F5F5;list-style: none;"><p> <strong>本文使用 <a href="https://creativecommons.org/licenses/by-nc-sa/3.0/cn/">CC BY-NC-SA 3.0 中国大陆</a> 协议许可<br>具体请参见 <a href="https://zyhang8.github.io/agree/">知识共享协议</a></strong><br> <strong>本文链接：</strong><a href="https://zyhang8.github.io/2019/07/30/opencv-vs2019-win-config/">https://zyhang8.github.io/2019/07/30/opencv-vs2019-win-config/</a></p></blockquote>
