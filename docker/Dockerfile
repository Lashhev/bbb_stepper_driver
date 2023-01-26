FROM arm64v8/ubuntu:latest as intermediate
RUN apt update &&  apt install -y git
# add credentials on build
ARG SSH_PRIVATE_KEY
ARG SSH_PUBLIC_KEY
USER root
RUN mkdir /root/.ssh/
RUN echo "${SSH_PRIVATE_KEY}" > /root/.ssh/id_rsa
RUN echo "${SSH_PUBLIC_KEY}" > /root/.ssh/id_rsa.pub
# COPY beaglebone.pub /root/.ssh/
RUN chmod 700 /root/.ssh/id_rsa
RUN chmod 700 /root/.ssh/id_rsa.pub
# make sure your domain is accepted
RUN touch /root/.ssh/known_hosts
RUN ssh-keyscan github.com >> /root/.ssh/known_hosts

WORKDIR /home/user

RUN git clone --recursive git@github.com:Lashhev/bbb_stepper_driver.git

FROM arm64v8/ubuntu:latest 
LABEL version="1.0"
LABEL maintainer="Andrei Lashchev <lashchev-andrej@yandex.ru>"
LABEL description="A docker image contains a realization of Software stepper driver via BeagleBone Black GPIO for armv8"

RUN apt update &&  apt install -y build-essential libboost-dev libboost-program-options-dev openssl cmake && apt autoremove -y && rm -rf /var/lib/apt/lists/*
RUN useradd -rm -d /home/user -s /bin/bash -g root -G sudo -u 1001 user -p "$(openssl passwd -1 user)"

# USER user
COPY --from=intermediate /home/user/bbb_stepper_driver /home/user/bbb_stepper_driver

RUN cd /home/user/bbb_stepper_driver && mkdir build && cd build && cmake .. -DBUILD_STEPPER_DRIVER_LIB_EXAMPLES=ON -DBBB_GPIO_BUILD_EXAMPLES=OFF && make 

