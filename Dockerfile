FROM debian:latest

########################################################
# Essential packages for remote debugging and login in
########################################################

RUN apt-get update \
  && apt-get upgrade -y \
  && apt-get install -y  \
      build-essential apt-utils openssh-server \
      gcc  g++ gdb gdbserver clang make ninja-build cmake \
      autoconf automake libtool valgrind locales-all \
      dos2unix rsync python3 rsync \
      vim curl zip unzip tar git pkg-config \
  && apt-get clean

RUN git clone https://github.com/microsoft/vcpkg /vcpkg
WORKDIR /vcpkg
RUN ls
# this is because run on arm chip
ENV VCPKG_FORCE_SYSTEM_BINARIES=1
RUN bash bootstrap-vcpkg.sh
RUN ./vcpkg install cpp-httplib nlohmann-json

ADD . /code
WORKDIR /code

# Taken from - https://docs.docker.com/engine/examples/running_ssh_service/#environment-variables

RUN mkdir /var/run/sshd
RUN echo 'root:root' | chpasswd
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config

# SSH login fix. Otherwise user is kicked off after login
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

ENV NOTVISIBLE="in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile

# 22 for ssh server. 7777 for gdb server.
EXPOSE 22 7777

RUN useradd -ms /bin/bash debugger
RUN echo 'debugger:pwd' | chpasswd

########################################################
# Add custom packages and development environment here
######8560497482f##################################################

########################################################

CMD ["/usr/sbin/sshd", "-D"]