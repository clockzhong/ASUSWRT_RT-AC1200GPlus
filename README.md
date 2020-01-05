### ASUSWRT_RT-AC1200G+

The official ASUS WRT for RT-AC1200+, copied the code source tree from: 
https://dlcdnets.asus.com/pub/ASUS/wireless/RT-AC1200G+/GPL_RT-AC1200GPlus_300438251610.zip

### Install Dependencies:
``` bash
sudo apt-get install --no-install-recommends autoconf automake bash bison bzip2 diffutils file flex g++ gawk gcc-multilib gettext gperf groff-base libncurses-dev libexpat1-dev libslang2 libssl-dev libtool libxml-parser-perl make patch perl pkg-config python sed shtool tar texinfo unzip zlib1g zlib1g-dev
sudo apt-get install lib32stdc++6 lib32z1-dev
sudo apt-get --no-install-recommends install automake1.11
sudo apt-get install libelf-dev:i386 libelf1:i386
sudo apt-get --no-install-recommends install lib32z1-dev lib32stdc++6
sudo apt-get install cmake xsltproc docbook-xsl-* intltool patch libglib2.0-dev gtk-doc-tools u-boot-tools texinfo autopoint
```
The about dependencies were verified on Ubuntu18

### Start Build:
``` bash
sudo su
source ./setEnv.sh
cd $SRC
make RT-AC1200G+
```

### Then upload ./image/RT-AC1200G+.trx to your router with its web interface, the following steps:
1. Browse your router in browser as address: 192.168.0.1
2. Login
3. Click "Adminstration"
4. Choose the page tab :"Firmware Upgrade" 
5. Besides the "New Firmware File", choose the file you just built, and click the "Upload" button, then wait for your router reboot done, and check its firmeware with the above steps to see the same page, and its version should be "3.0.0.4.382_51610***".

                        Clock  2019/04/01


