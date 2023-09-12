.extern _ZN5Riscv12ecallHandlerEv
.extern _ZN5Riscv12timerHandlerEv
.extern _ZN5Riscv14consoleHandlerEv


.align 4
.global _ZN5Riscv5TableEv
.type _ZN5Riscv5TableEv, @function
_ZN5Riscv5TableEv:
        j ecallHandle
        j timerHandle
        csrw mscratch, -1
        csrw mscratch, -1
        csrw mscratch, -1
        csrw mscratch, -1
        csrw mscratch, -1
        csrw mscratch, -1
        csrw mscratch, -1
        j consoleHandle

ecallHandle:
       addi sp, sp, -256
       .irp index,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
       sd x\index, \index*8(sp)
       .endr

       call _ZN5Riscv12ecallHandlerEv

       sd a0, 10*8(sp)
       .irp index,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
       ld x\index, \index*8(sp)
       .endr
       addi sp, sp, 256
       sret

timerHandle:
       addi sp, sp, -256
       .irp index,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
       sd x\index, \index*8(sp)
       .endr

       call _ZN5Riscv12timerHandlerEv

       .irp index,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
       ld x\index, \index*8(sp)
       .endr
       addi sp, sp, 256
       sret

consoleHandle:
       addi sp, sp, -256
       .irp index,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
       sd x\index, \index*8(sp)
       .endr

       call _ZN5Riscv14consoleHandlerEv

       sd a0, 10*8(sp)
       .irp index,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
       ld x\index, \index*8(sp)
       .endr
       addi sp, sp, 256
       sret
