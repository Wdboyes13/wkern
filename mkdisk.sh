#!/usr/bin/env bash
dd if=/dev/zero of=fat16.img bs=1M count=64
mkfs.fat -F 16 fat16.img