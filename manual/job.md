<head>
<title>Linux的重要性</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
</head>

# 为什么学习Linux

## 工作机会
1. 最热门的IT企业<br>
10多年前，微软、IBM<br>
今天，Google、Facebook、百度、阿里、腾讯

2. 互联网公司的业务构建在linux系统上，对Linux人才有较强的需求<br>
<table border=1 cellspacing=0 cellpadding=4>
<tr>
<td>公司</td>
<td>主要业务</td>
<td>客户端</td>
<td>服务端</td>
</tr>

<tr>
<td>百度</td>
<td>搜索引擎</td>
<td>通过浏览器访问</td>
<td>核心算法运行在服务器，页面索引保存在服务器</td>
</tr>

<tr>
<td>阿里</td>
<td>淘宝</td>
<td>通过浏览器访问</td>
<td>商品数据保存在服务器</td>
</tr>

<tr>
<td>腾讯</td>
<td>即时通讯</td>
<td>QQ、微信</td>
<td>联系人信息存在服务器</td>
</tr>
</table>

## 为什么服务器操作系统选择Linux，而不是Windows
1. 基于命令行的工作环境适用于服务器管理<br>
    - 易于自动化
        + 在某个指定目录下，查找符合条件的临时文件，并删除
        + 该任务需要每天都执行
        + GUI操作需要多次点击鼠标，每天都要重复这样的操作，太繁琐了
        + 体现出命令行的优势
        + 在/var/log目录下查找后缀为log并且文件大小超过1M的文件并删除<br>
        <pre>
          find /var/log -name '*.log' -size +1M -exec rm '{}' 
        </pre>
        + 创建1000个文件<br>
        <pre>
          i=0
          while [ $i -lt 1000 ]; do
              mkdir dir-$i
              i=$((i + 1))
          done
        </pre>

    - 远程维护方便
        + 缺省情况下，在机房的服务器不配置显示器和键盘
        + 节省成本和空间
        + 服务器在甲地出现故障，运维人员在乙地
        + 运维人员通过网络连接服务器，向服务器发送命令，并接受命令输出

2. 已经拥有良好的生态系统<br>
    - 服务器端的各种软件都为Linux而设计，默认在Linux上跑 
    - 首先出现在Linux，然后才移植到Windows

3. 相对稳定<br>
    - windows的GUI系统是集成在内核里，GUI中的BUG会导致系统崩溃
    - linux的GUI系统是运行在用户空间，是一个可选的部件，通常服务端不安装

4. 免费<br>
    - 国外有购买正版的传统，如果需要服务器节点个数很多，能节省很多授权费用。
