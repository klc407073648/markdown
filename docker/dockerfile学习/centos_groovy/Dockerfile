#基础镜像
FROM centos_base

#创建路径
RUN mkdir -p /home/data

#安装工具
RUN yum -y install zip unzip

#安装java和groovy
ADD ./package/jdk-8u221-linux-x64.tar.gz /usr/local
COPY ./package/apache-groovy-sdk-3.0.7.zip /usr/local

RUN cd /usr/local && unzip apache-groovy-sdk-3.0.7.zip 

#ADD ./start.sh /home/data

#添加环境变量到 /etc/profile
RUN echo 'export JAVA_HOME=/usr/local/jdk1.8.0_221' >> /etc/profile
RUN echo 'export JRE_HOME=$JAVA_HOME/jre'	 >> /etc/profile
RUN echo 'export CLASSPATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar' >> /etc/profile
RUN echo 'export PATH=$PATH:$JAVA_HOME/bin:$JRE_HOME/bin'	>> /etc/profile

RUN echo 'export GROOVY_HOME=/usr/local/groovy-3.0.7' >> /etc/profile
RUN echo 'export PATH=$PATH:$GROOVY_HOME/bin'	>> /etc/profile

#source 使得环境变量生效
RUN source /etc/profile

#解决重启配置不生效（source /etc/profile）
RUN echo 'source /etc/profile'	>> ~/.bashrc

#ENTRYPOINT /home/data/start.sh
