#include "core.h"

#include "inst.h"
#include "tick.h"
#include "arch.h"
#include "mem_map.h"
#include "common.h"

#include <stddef.h>
#include <assert.h>
#include <stdlib.h>

// -------- helper functions for immediates and sign-extension --------
static inline uint32_t sext32(uint32_t v, unsigned bits) {
    unsigned s = 32 - bits;
    return (uint32_t)((int32_t)(v << s) >> s);
}
static inline uint32_t imm_I(inst_fields_t f) {
    return sext32((uint32_t)f.I_TYPE.imm_11_0, 12);
}
static inline uint32_t imm_S(inst_fields_t f) {
    uint32_t imm = ((uint32_t)f.S_TYPE.imm_11_5 << 5) | f.S_TYPE.imm_4_0;
    return sext32(imm, 12);
}
static inline uint32_t imm_B(inst_fields_t f) {
    uint32_t imm = 0;
    imm |= ((uint32_t)f.B_TYPE.imm_12) << 12;
    imm |= ((uint32_t)f.B_TYPE.imm_10_5) << 5;
    imm |= ((uint32_t)f.B_TYPE.imm_4_1) << 1;
    imm |= ((uint32_t)f.B_TYPE.imm_11) << 11;
    return sext32(imm, 13);
}
static inline uint32_t imm_U(inst_fields_t f) {
    return ((uint32_t)f.U_TYPE.imm_31_12) << 12;
}
static inline uint32_t imm_J(inst_fields_t f) {
    uint32_t imm = 0;
    imm |= ((uint32_t)f.J_TYPE.imm_20) << 20;
    imm |= ((uint32_t)f.J_TYPE.imm_10_1) << 1;
    imm |= ((uint32_t)f.J_TYPE.imm_11) << 11;
    imm |= ((uint32_t)f.J_TYPE.imm_19_12) << 12;
    return sext32(imm, 21);
}

// ------------------------ Fetch ------------------------
static inst_fields_t Core_fetch(Core *self) {
    byte_t inst_in_bytes[4] = {};
    MemoryMap_generic_load(&self->mem_map, self->arch_state.current_pc, 4, inst_in_bytes);
    inst_fields_t ret = {};
    ret.raw |= (reg_t)inst_in_bytes[0];
    ret.raw |= (reg_t)inst_in_bytes[1] << 8;
    ret.raw |= (reg_t)inst_in_bytes[2] << 16;
    ret.raw |= (reg_t)inst_in_bytes[3] << 24;
    return ret;
}

// ------------------------ Decode ------------------------
static inst_enum_t Core_decode(Core *self, inst_fields_t f) {
    (void)self;
    inst_enum_t ret = {};

    uint32_t opcode = f.R_TYPE.opcode;
    uint32_t func3  = f.R_TYPE.func3;
    uint32_t func7  = f.R_TYPE.func7;

    switch (opcode) {
    case OP:
        switch (func3) {
        case 0x0: ret = (func7 == 0x20) ? inst_sub : inst_add; break;
        case 0x1: ret = inst_sll; break;
        case 0x2: ret = inst_slt; break;
        case 0x3: ret = inst_sltu; break;
        case 0x4: ret = inst_xor; break;
        case 0x5: ret = (func7 == 0x20) ? inst_sra : inst_srl; break;
        case 0x6: ret = inst_or; break;
        case 0x7: ret = inst_and; break;
        }
        break;

    case OP_IMM:
        switch (func3) {
        case 0x0: ret = inst_addi; break;
        case 0x2: ret = inst_slti; break;
        case 0x3: ret = inst_sltiu; break;
        case 0x4: ret = inst_xori; break;
        case 0x6: ret = inst_ori; break;
        case 0x7: ret = inst_andi; break;
        case 0x1: ret = inst_slli; break;
        case 0x5: ret = (func7 == 0x20) ? inst_srai : inst_srli; break;
        }
        break;

    case LOAD:
        switch (func3) {
        case 0x0: ret = inst_lb; break;
        case 0x1: ret = inst_lh; break;
        case 0x2: ret = inst_lw; break;
        case 0x4: ret = inst_lbu; break;
        case 0x5: ret = inst_lhu; break;
        }
        break;

    case STORE:
        switch (func3) {
        case 0x0: ret = inst_sb; break;
        case 0x1: ret = inst_sh; break;
        case 0x2: ret = inst_sw; break;
        }
        break;

    case BRANCH:
        switch (func3) {
        case 0x0: ret = inst_beq; break;
        case 0x1: ret = inst_bne; break;
        case 0x4: ret = inst_blt; break;
        case 0x5: ret = inst_bge; break;
        case 0x6: ret = inst_bltu; break;
        case 0x7: ret = inst_bgeu; break;
        }
        break;

    case JAL: ret = inst_jal; break;
    case JALR: ret = inst_jalr; break;
    case AUIPC: ret = inst_auipc; break;
    case LUI: ret = inst_lui; break;
    }
    return ret;
}

// ------------------------ Execute ------------------------
static void Core_execute(Core *self, inst_fields_t f, inst_enum_t e) {
    self->new_pc = self->arch_state.current_pc + 4;

    uint32_t rs1 = f.R_TYPE.rs1;
    uint32_t rs2 = f.R_TYPE.rs2;
    uint32_t rd  = f.R_TYPE.rd;

    uint32_t a   = self->arch_state.gpr[rs1];
    uint32_t b   = self->arch_state.gpr[rs2];
    uint32_t res = 0, addr;
    byte_t buf[4];

    switch (e) {
    // ---- R-type ----
    case inst_add: res = a + b; break;
    case inst_sub: res = a - b; break;
    case inst_sll: res = a << (b & 0x1F); break;
    case inst_slt: res = ((int32_t)a < (int32_t)b); break;
    case inst_sltu: res = (a < b); break;
    case inst_xor: res = a ^ b; break;
    case inst_srl: res = a >> (b & 0x1F); break;
    case inst_sra: res = (uint32_t)((int32_t)a >> (b & 0x1F)); break;
    case inst_or: res = a | b; break;
    case inst_and: res = a & b; break;

    // ---- OP-IMM ----
    case inst_addi: res = a + imm_I(f); break;
    case inst_slti: res = ((int32_t)a < (int32_t)imm_I(f)); break;
    case inst_sltiu: res = (a < (uint32_t)imm_I(f)); break;
    case inst_xori: res = a ^ imm_I(f); break;
    case inst_ori: res = a | imm_I(f); break;
    case inst_andi: res = a & imm_I(f); break;
    case inst_slli: res = a << (f.I_TYPE.imm_11_0 & 0x1F); break;
    case inst_srli: res = a >> (f.I_TYPE.imm_11_0 & 0x1F); break;
    case inst_srai:
        res = (uint32_t)((int32_t)a >> (f.I_TYPE.imm_11_0 & 0x1F));
        break;

    // ---- LOAD ----
    case inst_lb:
        addr = a + imm_I(f);
        MemoryMap_generic_load(&self->mem_map, addr, 1, buf);
        res = sext32(buf[0], 8);
        break;
    case inst_lh:
        addr = a + imm_I(f);
        MemoryMap_generic_load(&self->mem_map, addr, 2, buf);
        res = sext32(buf[0] | (buf[1] << 8), 16);
        break;
    case inst_lw:
        addr = a + imm_I(f);
        MemoryMap_generic_load(&self->mem_map, addr, 4, buf);
        res = buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
        break;
    case inst_lbu:
        addr = a + imm_I(f);
        MemoryMap_generic_load(&self->mem_map, addr, 1, buf);
        res = buf[0];
        break;
    case inst_lhu:
        addr = a + imm_I(f);
        MemoryMap_generic_load(&self->mem_map, addr, 2, buf);
        res = buf[0] | (buf[1] << 8);
        break;

    // ---- STORE ----
    case inst_sb:
        addr   = a + (uint32_t)imm_S(f);
        buf[0] = (byte_t)(b & 0xFF);
        if (addr == 0xFFFFFFFBu)
            exit(0); // manual HALT
        MemoryMap_generic_store(&self->mem_map, addr, 1, buf);
        break;
    case inst_sh:
        addr   = a + (uint32_t)imm_S(f);
        buf[0] = (byte_t)(b & 0xFF);
        buf[1] = (byte_t)((b >> 8) & 0xFF);
        MemoryMap_generic_store(&self->mem_map, addr, 2, buf);
        break;
    case inst_sw:
        addr   = a + (uint32_t)imm_S(f);
        buf[0] = (byte_t)(b & 0xFF);
        buf[1] = (byte_t)((b >> 8) & 0xFF);
        buf[2] = (byte_t)((b >> 16) & 0xFF);
        buf[3] = (byte_t)((b >> 24) & 0xFF);
        MemoryMap_generic_store(&self->mem_map, addr, 4, buf);
        break;

    // ---- BRANCH ----
    case inst_beq:
        if (a == b)
            self->new_pc = self->arch_state.current_pc + imm_B(f);
        break;
    case inst_bne:
        if (a != b)
            self->new_pc = self->arch_state.current_pc + imm_B(f);
        break;
    case inst_blt:
        if ((int32_t)a < (int32_t)b)
            self->new_pc = self->arch_state.current_pc + imm_B(f);
        break;
    case inst_bge:
        if ((int32_t)a >= (int32_t)b)
            self->new_pc = self->arch_state.current_pc + imm_B(f);
        break;
    case inst_bltu:
        if (a < b)
            self->new_pc = self->arch_state.current_pc + imm_B(f);
        break;
    case inst_bgeu:
        if (a >= b)
            self->new_pc = self->arch_state.current_pc + imm_B(f);
        break;

    // ---- Jumps ----
    case inst_jal:
        res          = self->arch_state.current_pc + 4;
        self->new_pc = self->arch_state.current_pc + imm_J(f);
        break;
    case inst_jalr:
        res          = self->arch_state.current_pc + 4;
        self->new_pc = (a + imm_I(f)) & ~1u;
        break;

    // ---- U-type ----
    case inst_auipc: res = self->arch_state.current_pc + imm_U(f); break;
    case inst_lui: res = imm_U(f); break;

    default: break;
    }

    // ---- Write-back ----
    switch (e) {
    case inst_sb:
    case inst_sh:
    case inst_sw:
    case inst_beq:
    case inst_bne:
    case inst_blt:
    case inst_bge:
    case inst_bltu:
    case inst_bgeu: break;
    default:
        if (rd != 0)
            self->arch_state.gpr[rd] = res;
        break;
    }

    // keep x0 = 0
    self->arch_state.gpr[0] = 0;
}

// ------------------------ Update PC ------------------------
static void Core_update_pc(Core *self) {
    self->arch_state.current_pc = self->new_pc;
}

// ------------------------ Ticking ------------------------
DECLARE_TICK_TICK(Core) {
    Core *self_               = container_of(self, Core, super);
    inst_fields_t inst_fields = Core_fetch(self_);
    inst_enum_t inst_enum     = Core_decode(self_, inst_fields);
    Core_execute(self_, inst_fields, inst_enum);
    Core_update_pc(self_);
}

// ------------------------ Constructor / Destructor ------------------------
void Core_ctor(Core *self) {
    assert(self != NULL);
    MemoryMap_ctor(&self->mem_map);
    Tick_ctor(&self->super);
    static struct TickVtbl const vtbl = { .tick = SIGNATURE_TICK_TICK(Core) };
    self->super.vtbl                  = &vtbl;
}
void Core_dtor(Core *self) {
    assert(self != NULL);
    MemoryMap_dtor(&self->mem_map);
}
int Core_add_device(Core *self, mmap_unit_t new_device) {
    return MemoryMap_add_device(&self->mem_map, new_device);
}
