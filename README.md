# Game Server Demo

# Manager 命令
- [x] add 添加客户端
  - [x] add number, 添加number个客户端
- [x] login 客户端登录
  - [x] login 无参数时全部登录
  - [x] login index, index客户端登录

# Client与Server通讯时的消息格式
格式介绍：  
>> number 账号  
>> id 用户id  
>> room_id 房间id  
>> \<number\> 表示数字  
>> "string" 表示字符串  
>> "" 空，无内容  

### 心跳  
heart \<id>

### 登录
login "number"
>> login_ok "id" "name" "number" "create_time"  
>> login_false ""

### 创建房间
room_create "user_id"
>> room_create_true "room_id"

### 邀请用户进入房间
room_invite \<room_id> \<id1> \<id2> user1邀请user2进入room_id
>> room_invite_true  
>> room_invite_false  


## 📌 功能进度

- [x] 封装网络库
  - [x] 基础 socket 封装（bind、listen、accept、connect、recv、send）
  - [x] TCP 长度前缀消息封装
  - [x] 非阻塞 socket 基础封装
  - [x] server/client 登录链路能跑通
  - [ ] 结合 epoll 优化封装（支持多连接）
- [x] 心跳
  - [x] 客户端定时发送心跳包
  - [x] 服务器刷新 Redis 在线状态 TTL
  - [x] Redis key 超时自动过期
  - [ ] 服务端主动感知并处理离线事件
- [x] 定时器
  - [x] 简单定时任务
  - [x] 支持周期性任务
  - [ ] 定时清理超时连接
  - [ ] 支持一次性延时任务（如延迟 3 秒执行）
  - [ ] 与心跳/战斗逻辑结合（驱动帧同步）
- [ ] 聊天系统
  - [ ] 客户端发送聊天消息
  - [ ] 服务器转发给同房间所有玩家
- [ ] 房间系统
  - [ ] Redis 存储房间信息（房间号、玩家列表）
  - [ ] 玩家加入房间
  - [ ] 玩家退出房间
- [ ] 匹配系统
  - [ ] 玩家点击匹配 → 加入 Redis 队列
  - [ ] 队列人数满足条件 → 创建房间
- [ ] 战斗逻辑（帧同步）
  - [ ] 客户端定时发送操作（如每 50ms）
  - [ ] 服务器收集并广播操作
  - [ ] 简单状态同步（如血量）
- [ ] 玩家数据管理
  - [x] MySQL 基础查询（SQLBuilder）
  - [x] Redis 基础键值操作
  - [ ] Redis 连接池
  - [x] 登录时加载基础用户数据
  - [ ] 登录时加载完整玩家数据（等级、金币）
  - [ ] 退出时保存玩家数据
- [x] 在线状态管理
  - [x] Redis 维护玩家在线状态：`online:user:{id}`
  - [x] 心跳刷新在线 TTL
  - [ ] 离线通知/清理逻辑
- [x] 日志系统
  - [x] 异步写日志（专用线程 + 队列）
  - [ ] 按日期分文件保存
  - [ ] 日志级别（INFO / WARN / ERROR）
