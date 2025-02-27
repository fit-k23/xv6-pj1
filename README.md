## 1. Usage
### 1.1 Setup: Ubuntu - WSL
1. `sudo apt update`
2. `sudo apt upgrade`
3. `sudo apt install git build-essential qemu-system gcc-riscv64-linux-gnu`
4. `git clone https://github.com/fit-k23/xv6-pj1.git xv6`
5. `cd xv6`
6. `make`
7. `make qemu`
8. xv6 is launched!
### 1.2 Exit xv6
1. Press `Ctrl + A` then press `X` to exit.
### 1.2b Exit xv6
1. Press `Ctrl + A` then release and quickly press `C` to enter QEMU terminal line.
2. Press `Q` for quit.
### 1.3 Run xv6
1. Run `make` to rebuilt changes
2. Run `make qemu` to run QEMU and xv6
