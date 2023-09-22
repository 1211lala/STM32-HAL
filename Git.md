<!--
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-09-19 20:11:10
 * @LastEditors: liuao 2494210546@qq.com
 * @LastEditTime: 2023-09-19 21:47:36
 * @FilePath: \undefinedc:\Users\liuao\Desktop\Git.md
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
-->

# Git常用操作

## 账户设置
### git -v                                                   查看版本信息
### config --global user.name "liuao"                        配置用户使用名
### git config --global user.email "2494210546@qq.com"       配置用户邮箱  
### git config --global credential.helper store              保存配置信息
### git config --global --list                               查看配置信息   


## 1、创建仓库
### git init                            创建仓库
### git add .                           添加所有文件到缓存区
### git status                          查看缓存区状态
### git commit -m '备注信息'            添加提交信息备注
### git log                             查看提交记录
### git log --oneline                   查看提交记录的简短信息
### git reset --soft  记录码            保存工作区和缓存区的内容
### git reset --hard  记录码            清空工作区和缓存区的内容
### git reset --mixed 记录码            保存工作区和清除缓存区的内容
### rm 文件名                            删除工作区文件
### git rm 文件名                        删除工作区和缓存区里的文件
### git rm --cached 文件名               删除缓存区不删除工作区
### git rm -r*文件名                     删除目录下所有文件，要提交


## 2、链接远端的仓库如github
> 1. 使用 ssh-keygen -t rsa -C "2494210546@qq.com"  得到密钥 路径在C:\Users\liuao\.ssh的id_rsa.pub文件
> 2. 在github中添加密钥
> 3. 使用 git clone url 
> 4. 使用 git push 推送本地仓库到远端


## 关联本地仓库到远端
> 1. 首先确定git初始化
> 2. 在github中添加密钥
> 3. 添加.gitignore文件减小工程大小
> 4. 使用 git add 和 git commit 提交要上传的文件 
> 4. git remote add origin url  建立联系
> 5. git push -u origin master  同步文件









