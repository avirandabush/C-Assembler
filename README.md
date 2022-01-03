# Assembler in c
university project, of assembler writing in c

imaginary computer with assembly language to his processor.

the processor:
  8 registers: r0 - r7, 15bit each
  1 register named: PSW (Program Status Word)
  memory: 4096 cells, 15bit each. every cell called "word".
  number renge: Z. only positive and negative numbers.
  arithmetic: two's complement
  chars: ascii

instruction set:
  mov = move
  cmp = compare
  add = add
  sub = subtruct
  lea = load effective address
  clr = clear
  not = not
  inc = increment
  dec = decrease
  jmp = jump
  bne = brunch if not equal
  red = read
  prn = print
  jsr = jump to procedure
  rts = back from procedure
  stop = stop program
