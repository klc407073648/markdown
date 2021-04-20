#/bin/bash
docker rm $(docker ps -q -f status=exited)
docker rmi centos_groovy

dos2unix *.sh
chmod 777 *.sh

docker build -t centos_groovy .
