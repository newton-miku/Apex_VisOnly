由newton_miku制作

颜色解释

```c++
/*可见为绿色
不可见为红色
倒地敌人为蓝色*/
可见敌人为绿色
不可见敌人为护甲颜色（无甲/空甲也为白色）
倒地敌人为蓝色（闪烁）
敌人总血量低于50时会闪烁
```

使用方法

- 打开寄汤入口.bat查看提示即可
  警告：程序启动过程中请勿同时启动游戏，避免关闭图片时误杀游戏
- 关于手机查看观战名单，默认ws端口为8090，可使用http://www.websocket-test.com/ 或者[apex.ddxnb.tk](https://apex.ddxnb.tk)进行查看,地址为开启软件的设备的局域网ip地址+默认端口8090。例如：我的电脑局域网ip为192.168.31.15，则网页中输入的地址为ws://192.168.31.15:8090(关于websocket的使用，请自行查阅相关资料)
- 关于自瞄：请按住键盘右Alt或者打开键盘大写锁定
- ~~如若提示缺少opencv_world460.dll~~
  ~~请前往以下地址下载~~
  ~~https://wws.lanzoul.com/iHcth09ira3i~~
  ~~密码:1234~~
  ~~下载后将dll文件放入C:\Windows\system32中~~

更新日志

```
v1.3.0.3 2022/10/14
更新：
	更新偏移：更新驱动
新增：
	顶级瞄准
V1.3.0.2 2022/9/30
更新：
	更新偏移，更新驱动
V1.3.0.1 2022/9/21
更新：
	游戏更新，更新偏移地址
	更新驱动，避免特征
V1.3.0.0 2022/9/14
更新：
	优化细节，不做说明
V1.2.6.8 2022/9/7
更新：
	游戏更新，更新偏移（其实没更）
V1.2.6.7 2022/9/6
更新：
	去除自瞄功能（后期购买kmbox后再重新上线）
V1.2.6.6 2022/8/31(本次为测试版本，仅用于测试自瞄功能的可行性)
更新：
	增加自瞄功能
优化：
	优化版本在线验证逻辑
V1.2.6.5 2022/8/28
更新：
	更新驱动特征
V1.2.6.4 2022/8/23
更新：
	更新偏移地址
V1.2.6.3 2022/8/22
更新：
	新增websocket查看观战人数
V1.2.6.2 2022/8/16
更新：
	更新驱动特征
V1.2.6.1 2022/8/14
优化：
	优化二维码打开逻辑，不再需要opencv的dll依赖
V1.2.6.0 2022/8/13
更新：
	驱动加载整合到exe文件中
优化：
	优化程序大小
V1.2.5.5 2022/8/12
修复：
	版本验证地址增加https支持
更新：
	将QR程序打包到主程序中
优化：
	优化了版本验证功能的语言逻辑等
V1.2.5.4 2022/8/10
修复：
	修复了版本验证地址1失效时无法使用的问题
V1.2.5.3 2022/8/10
更新：
	更新驱动特征
	S14更新偏移地址
V1.2.5.2 2022/8/6
更新：
	更新驱动特征
	增加备用版本验证地址
V1.2.5.1 2022/8/4
更新：
	增加版本验证与更新检测
V1.2.5.0 2022/7/31
更新：
	更新驱动特征
V1.2.4.7 2022/7/29
优化：
	优化了QR程序的体积及其界面逻辑
	QR程序现在会自动关闭图片（警告：程序启动过程中请勿同时启动游戏，避免关闭图片时误杀游戏）
更新：
	更新偏移地址（apex已经实装Hyperion）
计划：
	计划将QR扫码验证程序集成到主程序中
V1.2.4.6 2022/7/16
修复：
	游戏启动后才会再初始化驱动，避免驱动初始化错误
V1.2.4.5 2022/7/15
更新：
	更新偏移地址
	观众显示功能基本完成
	修改驱动名称，避免被特征
修复：
	本次游戏更新后敌人发光需要对发光时间进行修改
V1.2.4.2 2022/7/9
更新：
	新增观众显示（F7)开关命令行及观众显示（测试中）
计划：
	预计下次版本更新将修改驱动，避免被特征
V1.2.4 2022/7/2
更新：
	修改验证方式与逻辑，优化用户体验（新增手动验证方式，机器码不变的情况下仅第一次启动需要扫码）
优化：
	关闭玩家发光后，敌人会立马取消发光，而不是一段时间后取消发光
修复：
	修复关闭敌人发光后无法再次开启的问题
V1.2.3.2 2022/7/1
更新：
	游戏更新，更新偏移地址
	更新bat文件逻辑，方便一键打开压枪宏与程序
V1.2.3.1 2022/6/30
更新：
	优化调试逻辑，方便测试，功能上并无差别
V1.2.3 2022/6/30
更新：
	修改发光颜色逻辑：如果敌人护甲值为0，则显示白色
	增加热键方便开关发光与护甲发光
V1.2.2 2022/6/29
更新：
	尝试修改队友的发光类型
	（如果无法穿墙显示队友，请使用备份文件夹中的V1.2)
V1.2.1
	内部测试，不做更新日志
V1.2 2022/6/22
更新：
	将不可见的敌人改为护甲颜色
	可见敌人仍为绿色，倒地敌人改为蓝色（闪烁）
	更改发光距离（从500m更改为1200m）
V1.1 2022/6/14
更新：
	每30s检测一次游戏是否存在，如果不存在则退出程序，避免驱动长时间被无效占用
	优化发光颜色
V1.0.1 2022/6/13
更新：
	修改图标，便于区分
	程序加载完成后会自动隐藏命令行窗口
V1.0 2022/6/12
	更换驱动
	完成发光显示功能
	完成区分敌人是否可见及倒地状态
	增加qq验证功能
```
