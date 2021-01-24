FROM ubuntu:18.04

RUN apt update && \
		apt install -y -qq --no-install-recommends \
		git \
		vim \
		build-essential \
		libsdl2-dev \
		libsdl2-net-dev \
		libsdl2-image-dev \
		wget && \
		rm -rf /var/lib/apt/lists/*

RUN groupadd -g 1000 pokitto
RUN useradd -d /home/pokitto -s /bin/bash -m pokitto -u 1000 -g 1000

RUN cd /home/pokitto && \
		wget --no-check-certificate -nv https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2019q4/gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2 && \
		tar -xjf gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2 && \
		cp -r gcc-arm-none-eabi-9-2019-q4-major/* /usr && \
		rm -rf gcc-arm-none-eabi-9-2019-q4-major && \
		rm gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2

USER pokitto
ENV HOME /home/pokitto

RUN cd /home/pokitto && \
		git config --global http.sslVerify false && \
		git clone https://github.com/felipemanga/PokittoEmu.git && \
		cd PokittoEmu && \
		make -j ${nproc} && \
		ls BUILD/PokittoEmu

RUN cd /home/pokitto && \
		git clone https://github.com/pokitto/PokittoLib.git && \
		cd PokittoLib && \
		git reset --hard 4db8133bda37c3f086d3a9bde4f37e1916cb09e5

RUN cd /home/pokitto && \
		git clone https://github.com/halworsen/turbo-frog-seducer.git && \
		arm-none-eabi-gcc --version && \
		cd turbo-frog-seducer/tfs && \
		make clean && \
		make -j ${nproc} -k
