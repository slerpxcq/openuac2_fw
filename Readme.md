# OpenUAC2
An open-source audio streaming bridge compatible with USB Audio 2.0 specification

## Features
- Full USB 2.0 HS support
- Very low cost: no external programmable logic required
- Easy interfacing with DACs
- PCM supports: **44.1k, 48k, 88.2k, 96k, 176.4k, 192k, 352.8k, ~~384k~~(Work in progress)**
- DoP supports: **DSD64, DSD128**

## Current limitations
- PCM 384k does not work
- DSD native is unsupported due to driver
- Manual reset may be required after reconnecting USB cable