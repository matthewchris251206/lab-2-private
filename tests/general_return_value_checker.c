#include <iss.h>
#include <arch.h>

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    ISS *iss_ptr = NULL;
    ISS_ctor(&iss_ptr, argv[1]);
    while (!ISS_get_halt(iss_ptr)) {
        ISS_step(iss_ptr, 1);
    }

    arch_state_t state = ISS_get_arch_state(iss_ptr);
    printf("The return value from verifier in register $a0 is %d\n", state.gpr[10]);
    if (state.gpr[10] != 0) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
