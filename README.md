TODO:
 - добавить работу с gdb (скрипты и описание)

build:
    make build

clean:
    make clean

write flash:
    make flash
    ...OR..
    st-flash write ./out/hexbin/f407_empty.bin 0x8000000

read flash:
    st-flash read ./firmware.bin 0x8000000

erase flash:
    st-flash erase
