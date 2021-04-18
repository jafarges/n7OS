#include <inttypes.h>
#include <n7OS/irq.h>


void init_irq_entry(int irq_num, uint32_t addr) {
    idt_entry_t mEntry;

    mEntry.type_attr=128+14;
    mEntry.sel_segment = KERNEL_CS;
    mEntry.offset_inf =addr & 0xFFFF;
    mEntry.offset_sup=addr >> 16;
    idt[irq_num]=mEntry.offset_inf+(mEntry.sel_segment<<16)+((uint64_t)mEntry.type_attr<<40)+((uint64_t)mEntry.offset_sup<<48);
}
