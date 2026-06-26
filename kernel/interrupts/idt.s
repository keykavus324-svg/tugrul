.global idt_load
.global isr0
.global isr1
.global isr2
.global isr3
.global isr4
.global isr5
.global isr6
.global isr7
.global isr8
.global isr9
.global isr10
.global isr11
.global isr12
.global isr13
.global isr14
.global isr15
.global isr16
.global isr17
.global isr18
.global isr19
.global isr20
.global isr21
.global isr22
.global isr23
.global isr24
.global isr25
.global isr26
.global isr27
.global isr28
.global isr29
.global isr30
.global isr31
.global irq0
.global irq1
.global irq2
.global irq3
.global irq4
.global irq5
.global irq6
.global irq7
.global irq8
.global irq9
.global irq10
.global irq11
.global irq12
.global irq13
.global irq14
.global irq15
.global isr_common_stub
.global irq_common_stub

idt_load:
    movl 4(%esp), %eax
    lidt (%eax)
    ret

isr0: pushl $0; pushl $0; jmp isr_common_stub
isr1: pushl $0; pushl $1; jmp isr_common_stub
isr2: pushl $0; pushl $2; jmp isr_common_stub
isr3: pushl $0; pushl $3; jmp isr_common_stub
isr4: pushl $0; pushl $4; jmp isr_common_stub
isr5: pushl $0; pushl $5; jmp isr_common_stub
isr6: pushl $0; pushl $6; jmp isr_common_stub
isr7: pushl $0; pushl $7; jmp isr_common_stub
isr8: pushl $8; jmp isr_common_stub
isr9: pushl $0; pushl $9; jmp isr_common_stub
isr10: pushl $10; jmp isr_common_stub
isr11: pushl $11; jmp isr_common_stub
isr12: pushl $12; jmp isr_common_stub
isr13: pushl $13; jmp isr_common_stub
isr14: pushl $14; jmp isr_common_stub
isr15: pushl $0; pushl $15; jmp isr_common_stub
isr16: pushl $0; pushl $16; jmp isr_common_stub
isr17: pushl $0; pushl $17; jmp isr_common_stub
isr18: pushl $0; pushl $18; jmp isr_common_stub
isr19: pushl $0; pushl $19; jmp isr_common_stub
isr20: pushl $0; pushl $20; jmp isr_common_stub
isr21: pushl $0; pushl $21; jmp isr_common_stub
isr22: pushl $0; pushl $22; jmp isr_common_stub
isr23: pushl $0; pushl $23; jmp isr_common_stub
isr24: pushl $0; pushl $24; jmp isr_common_stub
isr25: pushl $0; pushl $25; jmp isr_common_stub
isr26: pushl $0; pushl $26; jmp isr_common_stub
isr27: pushl $0; pushl $27; jmp isr_common_stub
isr28: pushl $0; pushl $28; jmp isr_common_stub
isr29: pushl $0; pushl $29; jmp isr_common_stub
isr30: pushl $0; pushl $30; jmp isr_common_stub
isr31: pushl $0; pushl $31; jmp isr_common_stub

irq0: pushl $0; pushl $32; jmp irq_common_stub
irq1: pushl $0; pushl $33; jmp irq_common_stub
irq2: pushl $0; pushl $34; jmp irq_common_stub
irq3: pushl $0; pushl $35; jmp irq_common_stub
irq4: pushl $0; pushl $36; jmp irq_common_stub
irq5: pushl $0; pushl $37; jmp irq_common_stub
irq6: pushl $0; pushl $38; jmp irq_common_stub
irq7: pushl $0; pushl $39; jmp irq_common_stub
irq8: pushl $0; pushl $40; jmp irq_common_stub
irq9: pushl $0; pushl $41; jmp irq_common_stub
irq10: pushl $0; pushl $42; jmp irq_common_stub
irq11: pushl $0; pushl $43; jmp irq_common_stub
irq12: pushl $0; pushl $44; jmp irq_common_stub
irq13: pushl $0; pushl $45; jmp irq_common_stub
irq14: pushl $0; pushl $46; jmp irq_common_stub
irq15: pushl $0; pushl $47; jmp irq_common_stub

isr_common_stub:
    pusha
    movw %ds, %ax
    pushl %eax
    movl $0x10, %eax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    pushl %esp
    call isr_handler
    addl $4, %esp
    popl %eax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    popa
    addl $8, %esp
    iret

irq_common_stub:
    pusha
    movw %ds, %ax
    pushl %eax
    movl $0x10, %eax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    pushl %esp
    call irq_handler
    addl $4, %esp
    popl %eax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    popa
    addl $8, %esp
    iret
