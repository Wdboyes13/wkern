#include <types/nums.h>
typedef struct {
    u8 ip[4];
    u8 netmasK[4];
    u8 gateway[4];
} net_config_t;

net_config_t net_config = {
    .ip = {10, 0, 2, 15},
    .netmasK = {255, 255, 255, 0},
    .gateway = {10, 0, 2, 2},
};

static u8 macaddr[6] = {0x52, 0x54, 0x00, 0x12, 0x34, 0x56};

