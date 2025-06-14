__attribute__((noreturn)) void triple_fault() {
    __asm__ volatile("lidt (0)");  // Load an invalid IDT
    __asm__ volatile("int $0x03"); // Cause interrupt = triple fault = CPU reset
    __builtin_unreachable();
}