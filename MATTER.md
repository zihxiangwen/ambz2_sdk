# Matter (previously CHIP) on AmebaZ2

## How to support Matter on Ameba SDK

https://github.com/hank820/ambz2_sdk/blob/with_Matter/MATTER_PORTING.md

## Get Ameba SDK & Matter SDK

    Test on Ubuntu 20.04

To check out this repository:

    git clone -b with_Matter --recurse-submodules https://github.com/hank820/ambz2_sdk.git

If you already have a checkout, run the following command to sync submodules recursively:

	git submodule update --init --recursive


## Set Matter Build Environment 

    cd third_party/connectedhomeip

    source scripts/bootstrap.sh

    source scripts/activate.sh

    > Find more details to setup linux build environment
    > https://github.com/hank820/connectedhomeip/blob/master/docs/BUILDING.md


## Make CHIP library by gn and Make lib_main.a
### all-cluster-app

    cd ambz2_sdk/project/realtek_amebaz2_v0_example/GCC-RELEASE

    source ToolPath.sh
	
    make lib_all

### lighting-app

    cd ambz2_sdk/project/realtek_amebaz2_v0_example/GCC-RELEASE

    source ToolPath.sh

    make light

### CHIP core (generate by GN/ninja in connectedhomeip. Config by [lib_chip.mk](https://github.com/hank820/ambz2_sdk/blob/with_Matter/project/realtek_amebaz2_v0_example/GCC-RELEASE/lib_chip.mk))

    output : ambz2_sdk/component/soc/realtek/8710c/misc/bsp/lib/common/GCC
	
    > libCHIP.a, ibCoreTests.a, ibChipCryptoTests.a, ibRawTransportTests.a...

### CHIP application (generate by [lib_chip_main.mk](https://github.com/hank820/ambz2_sdk/blob/with_Matter/project/realtek_amebaz2_v0_example/GCC-RELEASE/lib_chip_main.mk)))

    output : ambz2_sdk/component/soc/realtek/8710c/misc/bsp/lib/common/GCC

    > lib_main.a

## Make Ameba application
    cd ambz2_sdk/project/realtek_amebaz2_v0_example/GCC-RELEASE

    make is

## Flash Image
    ambz2_sdk/project/realtek_amebaz2_v0_example/GCC-RELEASE/application_is/Debug/bin/flash_is.bin

## Run CHIP task on AmebaZ2 (all-cluster-app/lighting-app example)
    enter command in console

    ATW0=testAP

    ATW1=password

    ATWC

    ATS$ => Run chip task


## Test with [chip-tool](https://github.com/hank820/connectedhomeip/tree/master/examples/chip-tool)
Use standalone chip-tool app(linux) to communicate with the device.

`Disable "config_pair_with_random_id = false" in examples/chip-tool/BUILD.gn if test bypass mode`

`./chip-tool pairing bypass xxx.xxx.xxx.xxx 5540  (Ameba IP)`

<b>onoff cluster</b>

Use PA_20 as output, connect a LED to this pin and GND.

`./chip-tool onoff on 1`

`./chip-tool onoff off 1`
    
<b>doorlock cluster</b>

`./chip-tool doorlock lock-door 1 1`
    
`./chip-tool doorlock unlock-door 1 1`

